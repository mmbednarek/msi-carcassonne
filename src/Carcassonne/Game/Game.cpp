#include <Carcassonne/Game/Game.h>
#include <Carcassonne/Game/Move.h>
#include <algorithm>
#include <fmt/core.h>
#include <memory_resource>
#include <set>

namespace carcassonne::game {

Game::Game() : m_random_generator(10101) {
   apply_tile(70, 70, 1, 3);
}

const IBoard &Game::board() const noexcept {
   return m_board;
}

Player Game::current_player() const noexcept {
   return m_current_player;
}

std::unique_ptr<IMove> Game::new_move(Player p) noexcept {
   return std::make_unique<Move>(p, (m_random_distribution(m_random_generator) % 24) + 1, *this);
}

mb::view<Figure> Game::figures() const noexcept {
   return m_figures;
}

void Game::add_figure(Figure f) {
   m_figures.push_back(f);
}

std::unique_ptr<IMove> Game::new_debug_move(Player p, TileType tt) noexcept {
   return std::make_unique<Move>(p, tt, *this);
}

void Game::apply_tile(int x, int y, TileType tt, mb::u8 rot) noexcept {
   m_board.set_tile(x, y, tt, rot);

   auto tile = TilePlacement{.type = tt, .rotation = rot}.tile();
   auto connections = tile.connections;
   while (connections != Connection::None) {
      Direction a, b;
      std::tie(connections, a, b) = read_directions(connections);
      m_groups.join(make_edge(x, y, a), make_edge(x, y, b));
   }
   mb::u8 buffer[sizeof(Group) * 8];
   std::pmr::monotonic_buffer_resource pool(std::data(buffer), std::size(buffer));
   std::pmr::set<Group> tile_groups(&pool);

   for (int direction_id = 0; direction_id < 4; ++direction_id) {
      auto direction = static_cast<Direction>(direction_id);
      auto edge = make_edge(x, y, direction);
      tile_groups.insert(m_groups.group_of(edge));
      if (m_board.tile_at(neighbour_of(x, y, direction)).type == 0) {
         m_groups.inc_free_edges(edge);
      } else {
         m_groups.dec_free_edges(edge);
      }
      m_groups.set_type(edge, tile.edges[direction_id]);
   }

   for (const auto group : tile_groups) {
      if (tile.pennant && m_groups.type_of(group) == EdgeType::Town) {
         m_groups.inc_tiles(group); // second time for the pennant
      }
      m_groups.inc_tiles(group);
   }

   for (mb::u8 direction_id = 5; direction_id < 13; ++direction_id) {
      m_groups.set_type(make_edge(x, y, static_cast<Direction>(direction_id)), tile.field_edges[direction_id - 5]);
   }

   std::pmr::set<Group> free_edges(&pool);
   for (mb::u8 direction_id = 0; direction_id < 4; ++direction_id) {
      auto direction = static_cast<Direction>(direction_id);
      auto edge = make_edge(x, y, direction);
      if (m_groups.type_of(edge) != EdgeType::Grass && m_groups.is_completed(edge)) {
         free_edges.insert(m_groups.group_of(edge));
      }
   }

   for (const auto edge : free_edges) {
      on_structure_completed(edge);
   }
}

void Game::on_structure_completed(Group g) {
   auto tile_type = m_groups.type_of(g);
   auto assignment = m_groups.assigment(g);
   Player player;
   switch (tile_type) {
   case EdgeType::Path:
      while (assignment != PlayerAssignment::None) {
         std::tie(assignment, player) = read_player_assignment(assignment);
         m_scores.add_points(player, m_groups.tile_count(g));
      }
      break;
   case EdgeType::Town:
      while (assignment != PlayerAssignment::None) {
         std::tie(assignment, player) = read_player_assignment(assignment);
         m_scores.add_points(player, 2 * static_cast<short>(m_groups.tile_count(g)));
      }
      break;
   default: break;
   }

   std::erase_if(m_figures,
                 [&groups = m_groups, target_group = g](const Figure &f) { return groups.group_of(f.edge) == target_group; });
}

const ScoreBoard &Game::scores() const noexcept {
   return m_scores;
}

}// namespace carcassonne::game