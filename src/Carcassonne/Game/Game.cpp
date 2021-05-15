#include <Carcassonne/Game/Game.h>
#include <Carcassonne/Game/Move.h>
#include <algorithm>
#include <fmt/core.h>
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

void Game::apply_tile(int x, int y, TileType tt, mb::u8 rot) noexcept {
   m_board.set_tile(x, y, tt, rot);

   auto tile = TilePlacement{.type = tt, .rotation = rot}.tile();
   auto connections = tile.connections;
   while (connections != Connection::None) {
      Direction a, b;
      std::tie(connections, a, b) = read_connections(connections);
      m_groups.join(make_edge(x, y, a), make_edge(x, y, b));
   }
   auto contacts = tile.contacts;
   while (contacts != Contact::None) {
      Direction a, b;
      std::tie(contacts, a, b) = read_contacts(contacts);
      Group town_group{m_groups.group_of(make_edge(x, y, a))};
      Group field_group{m_groups.group_of(make_edge(x, y, b))};
      auto is_pair_found = [town_group, field_group](std::pair<Group, Group> town_field) { return town_group == town_field.first && field_group == town_field.second; };
      if (auto result = std::find_if(m_towns.begin(), m_towns.end(), is_pair_found); result == m_towns.end())
         m_towns.push_back(std::make_pair(town_group, field_group));
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
         m_groups.inc_tiles(group);// second time for the pennant
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
      on_structure_completed(x, y, edge);
   }
}

void Game::on_structure_completed(int x, int y, Group g) {
   auto edge_type = m_groups.type_of(g);
   auto assignment = m_groups.assigment(g);
   Player player;
   switch (edge_type) {
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
   // if (mutable_board().tile_at(x, y).tile().monastery)
   //    if (mutable_board().tile_at(x, y).type == 6 && make_edge(x,  y, Direction::Middle))
   //       ;
   // else
   std::erase_if(m_figures, [&groups = m_groups, target_group = g](const Figure &f) { return groups.group_of(f.edge) == target_group; });
}

bool Game::is_monastery_completed(int x, int y) noexcept {
   Monastery mon;
   for (mb::u8 i = x - 1; i <= x + 1; i++)
      for (mb::u8 j = y - 1; j <= y + 1; j++)
         mon.push_back(mutable_board().tile_at(i, j).type);
   for (auto const &tileType : mon)
      if (tileType == static_cast<TileType>(0))
         return false;
   return true;
}

void Game::on_monastery_completed(int x, int y, Player player) {
   m_scores.add_points(player, 9);
   for (auto const &figure : m_figures)
      fmt::print("{} {} {} {} {}\n", figure.x, figure.y, x, y, figure.x == x + 0.5 && figure.y == y + 0.5 ? "true" : "false");
   std::erase_if(m_figures,
                 [&x, &y](const Figure &f) { return f.x == x + 0.5 && f.y == y + 0.5; });
   // std::erase_if(m_figures,
   //               [&groups = m_groups, target_group = g](const Figure &f) { return groups.group_of(f.edge) == target_group; });
}

const ScoreBoard &Game::scores() const noexcept {
   return m_scores;
}

bool Game::is_town_field_connected(Edge town, Edge field) const noexcept {
   return std::find(m_towns.cbegin(), m_towns.cend(), std::make_pair(m_groups.group_of(town), m_groups.group_of(field))) != m_towns.end();
}

}// namespace carcassonne::game