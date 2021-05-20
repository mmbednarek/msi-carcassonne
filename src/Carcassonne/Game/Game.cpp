#include <Carcassonne/Game/Game.h>
#include <Carcassonne/Game/Move.h>
#include <algorithm>
#include <fmt/core.h>
#include <set>

namespace carcassonne::game {

Game::Game() : m_random_generator(9) {
   apply_tile(70, 70, 1, 3);
   draw_tiles();
}

const IBoard &Game::board() const noexcept {
   return m_board;
}

Player Game::current_player() const noexcept {
   return m_current_player;
}

mb::u8 Game::move_nr() const noexcept {
   return m_move_nr;
}

bool Game::find_possible_moves(TileType tt) noexcept {
   m_possible_moves = PossibleMoves();
   bool possible_move_exists = false;
   for (int _x = board().min_x(); _x < board().max_x(); _x++) {
      for (int _y = board().min_y(); _y < board().max_y(); _y++) {
         for (mb::u8 rotation = 0; rotation < 4; ++rotation) {
            if (board().can_place_at(_x, _y, tt, rotation)) {
               m_possible_moves.push_back(PossibleMove(_x,_y,rotation));
               possible_move_exists = true;
            }
         }
      }
   }
   return possible_move_exists;
}

bool Game::can_place(TileType tt) noexcept {
   bool can_place = false;
   for (int _x = board().min_x() - 1; _x < board().max_x(); _x++) {
      for (int _y = board().min_y() - 1; _y < board().max_y(); _y++) {
         for (mb::u8 rotation = 0; rotation < 4; ++rotation) {
            if (board().can_place_at(_x, _y, tt, rotation)) {
               return true;
            }
         }
      }
   }
   return false;
}

std::unique_ptr<IMove> Game::new_move(Player p) noexcept {
   TileType tt = m_tile_set[m_move_nr];
   mb::u8 move_nr = m_move_nr;
   while (!can_place(tt) && m_move_nr != 0) {
      move_nr += m_player_count;
      tt = m_tile_set[move_nr];
      if (move_nr + m_player_count >= m_tile_set.size() - 1) { // if there is no tile to be swapped
         std::iter_swap(m_tile_set.begin() + m_move_nr, m_tile_set.begin() + m_move_nr + 1);
         move_nr = m_move_nr;
      }
   }
   if (m_move_nr != move_nr) {
      std::iter_swap(m_tile_set.begin() + m_move_nr, m_tile_set.begin() + move_nr);
   }
   tt = m_tile_set[m_move_nr++];
   return std::make_unique<Move>(p, tt, *this);
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

      auto town_group = m_groups.group_of(make_edge(x, y, a));
      auto field_group = m_groups.group_of(make_edge(x, y, b));

      if (!is_town_field_connected(town_group, field_group)) {
         m_towns.push_back(std::make_pair(town_group, field_group));
      }
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

   std::pmr::set<Group> completed_edges(&pool);
   for (mb::u8 direction_id = 0; direction_id < 4; ++direction_id) {
      auto direction = static_cast<Direction>(direction_id);
      auto edge = make_edge(x, y, direction);

      if (m_groups.type_of(edge) != EdgeType::Grass && m_groups.is_completed(edge)) {
         completed_edges.insert(m_groups.group_of(edge));
      }
   }

   for (const auto edge : completed_edges) {
      on_structure_completed(edge);
   }

   for (auto _x = x - 1; _x <= x + 1; _x++) {
      for (auto _y = y - 1; _y <= y + 1; _y++) {
         if (!mutable_board().tile_at(_x, _y).tile().monastery)
            continue;

         if (!is_monastery_completed(_x, _y))
            continue;

         Player player;
         std::tie(std::ignore, player) = read_player_assignment(m_groups.assigment(make_edge(_x, _y, Direction::Middle)));
         on_monastery_completed(_x, _y, player);
      }
   }
}

void Game::on_structure_completed(Group g) {
   switch (m_groups.type_of(g)) {
   case EdgeType::Path:
      m_scores.add_points(m_groups.assigment(g), m_groups.tile_count(g));
      break;
   case EdgeType::Town:
      m_scores.add_points(m_groups.assigment(g), static_cast<short>(2 * m_groups.tile_count(g)));
      break;
   default: break;
   }

   std::erase_if(m_figures,
                 [&groups = m_groups, target_group = g](const Figure &f) {
                    return groups.group_of(f.edge) == target_group;
                 });
}

bool Game::is_monastery_completed(int x, int y) noexcept {
   for (auto _x = x - 1; _x <= x + 1; _x++) {
      for (auto _y = y - 1; _y <= y + 1; _y++) {
         if (m_board.tile_at(_x, _y).type == 0) {
            return false;
         }
      }
   }
   return true;
}

void Game::on_monastery_completed(int x, int y, Player player) {
   m_scores.add_points(player, 9);
   std::erase_if(m_figures,
                 [&x, &y](const Figure &f) {
                    return f.x == x + 0.5 && f.y == y + 0.5;
                 });
}

const ScoreBoard &Game::scores() const noexcept {
   return m_scores;
}

bool Game::is_town_field_connected(Edge town, Edge field) const noexcept {
   return std::find(m_towns.cbegin(), m_towns.cend(), std::make_pair(m_groups.group_of(town), m_groups.group_of(field))) != m_towns.end();
}

template<class bidiiter>
bidiiter random_unique(bidiiter begin, bidiiter end, size_t num_random) {
    size_t left = std::distance(begin, end);
    while (num_random--) {
        bidiiter r = begin;
        std::advance(r, rand()%left);
        std::swap(*begin, *r);
        ++begin;
        --left;
    }
    return begin;
}

void Game::draw_tiles() {
   TileType tt = 0;
   for(const auto& tile : g_tiles) {
      for(size_t i = 0; i < tile.amount; i++) {
         m_tile_set.push_back(static_cast<TileType>(tt));
      }
      tt++;
   }
   std::shuffle(m_tile_set.begin(), m_tile_set.end(), m_random_generator);
}

}// namespace carcassonne::game