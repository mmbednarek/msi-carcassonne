#include <Carcassonne/Game/Game.h>
#include <Carcassonne/Game/Move.h>
#include <algorithm>
#include <fmt/core.h>
#include <set>

namespace carcassonne::game {

Game::Game(int player_count, mb::u64 seed) : m_random_generator(seed),
                                             m_player_count(player_count) {
   std::fill(m_figure_count.begin(), m_figure_count.end(), 7);
   apply_tile(70, 70, 1, 3);
   draw_tiles();
}

const IBoard &Game::board() const noexcept {
   return m_board;
}

Player Game::current_player() const noexcept {
   return m_current_player;
}

mb::u8 Game::move_index() const noexcept {
   return m_move_index;
}

const TileSet &Game::tile_set() const noexcept {
   return m_tile_set;
}

bool Game::can_place(TileType tt) const noexcept {
   auto move_range = moves(tt);
   return move_range.begin() != move_range.end();
}

std::unique_ptr<IMove> Game::new_move(Player p) noexcept {
   auto tt = m_tile_set[m_move_index];
   auto move_index = m_move_index;
   while (m_move_index != 0 && m_move_index < m_tile_set.size() - 1 && !can_place(tt)) {
      move_index += m_player_count;
      tt = m_tile_set[move_index];
      if (move_index + m_player_count >= m_tile_set.size() - 1) {// if there is no tile to be swapped
         mb::u8 rotations = m_tile_set.size() - move_index;
         while (!can_place(m_tile_set[m_move_index])) {
            std::rotate(m_tile_set.begin() + m_move_index, m_tile_set.begin() + m_move_index + 1, m_tile_set.end());
            if (--rotations == 0) {// if went back to the tile with which rotating started
               m_move_index++;
               break;
            }
         }
         move_index = m_move_index;
      }
   }
   if (m_move_index != move_index) {
      std::iter_swap(m_tile_set.begin() + m_move_index, m_tile_set.begin() + move_index);
   }
   tt = m_tile_set[m_move_index++];
   return std::make_unique<Move>(p, tt, *this);
}

mb::view<Figure> Game::figures() const noexcept {
   return m_figures;
}

void Game::add_figure(Figure f) {
   --m_figure_count[static_cast<mb::size>(f.player)];
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

   m_groups.set_type(make_edge(x, y, Direction::Middle), EdgeType::Monastery);

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
                 [this, &groups = m_groups, target_group = g](const Figure &f) {
                    if (groups.group_of(f.edge) == target_group) {
                       ++m_figure_count[static_cast<mb::size>(f.player)];
                       return true;
                    }
                    return false;
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
                 [&x, &y, this](const Figure &f) {
                    if (f.tile_x == x && f.tile_y == y) {
                       ++m_figure_count[static_cast<mb::size>(f.player)];
                       return true;
                    }
                    return false;
                 });
}

const ScoreBoard &Game::scores() const noexcept {
   return m_scores;
}

bool Game::is_town_field_connected(Edge town, Edge field) noexcept {
   std::transform(m_towns.begin(), m_towns.end(), m_towns.begin(), [this](std::pair<Edge, Edge> &pair) {
      return std::make_pair(static_cast<Edge>(m_groups.group_of(pair.first)), static_cast<Edge>(m_groups.group_of(pair.second)));
   });
   return std::find(m_towns.cbegin(), m_towns.cend(), std::make_pair(static_cast<Edge>(m_groups.group_of(town)), static_cast<Edge>(m_groups.group_of(field)))) != m_towns.end();
}

void Game::draw_tiles() {
   TileType tt = 0;
   for (const auto &tile : g_tiles) {
      for (size_t i = 0; i < tile.amount; i++) {
         m_tile_set.push_back(static_cast<TileType>(tt));
      }
      tt++;
   }
   std::shuffle(m_tile_set.begin(), m_tile_set.end(), m_random_generator);
}

void Game::on_next_move(NextMoveCallback callback) noexcept {
   m_next_move_callbacks.push_back(callback);
}

std::vector<Direction> Game::figure_placements(int x, int y) const noexcept {
   std::vector<Direction> result;
   result.reserve(13);
   for (const auto dir : g_directions) {
      if (!can_place_figure(x, y, dir))
         continue;

      result.push_back(dir);
   }

   std::array<bool, 13> removed{};
   auto tile_placement = m_board.tile_at(x, y);
   auto connections = g_tiles[tile_placement.type].rotate(tile_placement.rotation).connections;
   while (connections != Connection::None) {
      Direction a, b;
      std::tie(connections, a, b) = read_connections(connections);

      if (removed[static_cast<mb::size>(a)] || removed[static_cast<mb::size>(b)])
         continue;

      removed[static_cast<mb::size>(a)] = true;
      std::erase(result, a);
   }

   return result;
}

bool Game::can_place_figure(int x, int y, Direction d) const {
   if (d == Direction::Middle) {
      return m_board.tile_at(x, y).tile().monastery;
   }

   auto edge = make_edge(x, y, d);
   auto edge_type = m_groups.type_of(edge);

   switch (d) {
   case Direction::North:
   case Direction::East:
   case Direction::South:
   case Direction::West:
      if (edge_type == EdgeType::Grass)
         return false;
      break;
   case Direction::EastNorth:
   case Direction::NorthEast:
   case Direction::SouthEast:
   case Direction::EastSouth:
   case Direction::WestSouth:
   case Direction::SouthWest:
   case Direction::NorthWest:
   case Direction::WestNorth:
      if (edge_type != EdgeType::Grass)
         return false;
      break;
   }

   return m_groups.is_free(edge);
}

void Game::start() noexcept {
   for (const auto &callback : m_next_move_callbacks) {
      callback(*this, m_current_player, m_last_move);
   }
}

void Game::set_next_player() noexcept {
   if (m_move_index >= m_tile_set.size()) {
      if (!m_game_finished) {
         m_game_finished = true;
         assign_final_points();
      }
      return;
   }

   m_current_player = next_player(m_current_player, m_player_count);
   for (const auto &callback : m_next_move_callbacks) {
      callback(*this, m_current_player, m_last_move);
   }
}

void Game::assign_final_points() noexcept {
   for (const auto figure : m_figures) {
      auto tile_type = m_groups.type_of(figure.edge);

      switch (tile_type) {
      case EdgeType::Grass:
         continue;
      case EdgeType::Monastery: {
         short point_count{};
         for (auto y = figure.tile_y - 1; y <= figure.tile_y + 1; ++y) {
            for (auto x = figure.tile_x - 1; x <= figure.tile_x + 1; ++x) {
               if (x == figure.tile_x && y == figure.tile_y)
                  continue;
               if (m_board.tile_at(x, y).type == 0)
                  continue;

               ++point_count;
            }
         }
         m_scores.add_points(figure.player, point_count);
         continue;
      }
      default:
         m_scores.add_points(figure.player, m_groups.tile_count(figure.edge));
         break;
      }
   }

   for (const auto pair : m_towns) {
      Edge town, field;
      std::tie(town, field) = pair;

      if (!m_groups.is_completed(town))
         continue;

      m_scores.add_points(m_groups.assigment(field), 3);
   }
}

mb::u8 Game::player_figure_count(Player p) const noexcept {
   return m_figure_count[static_cast<mb::size>(p)];
}

void Game::update(double dt) noexcept {
   if (m_tour_finished) {
      m_tour_finished = false;
      m_last_move = m_performed_move;
      set_next_player();
   }
}

void Game::notify_tour_finished(FullMove full_move) noexcept {
   m_tour_finished = true;
   m_performed_move = full_move;
}

std::unique_ptr<IGame> Game::clone() const noexcept {
   auto game = std::make_unique<Game>(*this);
   game->m_next_move_callbacks.clear();
   return game;
}

static int score_monastery(const Board &board, int x, int y) {
   int score = 0;
   for (auto cy = y - 1; cy <= y + 1; ++cy) {
      for (auto cx = x - 1; cx <= x + 1; ++cx) {
         if (cx == x && cy == y)
            continue;

         if (board.tile_at(cx, cy).type != 0)
            score += 50;
      }
   }
   return score;
}

int Game::score_grass(Player player, Edge edge) const noexcept {
   if (!m_groups.is_free(edge))
      return -20;

   if (player_figure_count(player) < 4)
      return 0;

   const auto count = std::count_if(m_towns.begin(), m_towns.end(), [this, expected = m_groups.group_of(edge)](std::pair<Edge, Edge> town_grass_pair) {
      if (!m_groups.is_completed(town_grass_pair.first))
         return false;
      return town_grass_pair.second == expected;
   });
   return static_cast<int>(count);
}

struct EdgeState {
   int opens{};
   int closes{};
   bool free{};
   bool our{};
   bool ignore{};
   int tile_count{};
   EdgeType type{};
};

static bool owns_edges(bool free_a, bool owns_a, bool free_b, bool owns_b) {
   if (!free_a && !owns_a)
      return false;
   if (!free_b && !owns_b)
      return false;
   return owns_a || owns_b;
}

int Game::score_tile(Player player, const Tile &tile, TileMove move, Direction target_direction) const noexcept {
   std::array<EdgeState, 4> edge_states;
   for (auto i = 0; i < 4; ++i) {
      auto direction = static_cast<Direction>(i);
      auto edge = make_edge(move.x, move.y, direction);

      auto neighbour = neighbour_of(move.x, move.y, direction);
      auto open = m_board.tile_at(neighbour.x, neighbour.y).type == 0;
      auto free = m_groups.is_free(edge);
      bool our;
      if (free) {
         our = (direction == target_direction) && player_figure_count(player) != 0;
      } else {
         our = m_groups.assigment(edge) & player;
      }

      edge_states[i] = EdgeState{
              .opens = open ? 1 : 0,
              .closes = open ? 0 : 1,
              .free = free,
              .our = our,
              .ignore = false,
              .tile_count = m_groups.tile_count(edge),
              .type = m_groups.type_of(edge),
      };
   }

   auto connections = tile.connections;
   while (connections != Connection::None) {
      Direction a, b;
      std::tie(connections, a, b) = read_connections(connections);

      auto ai = static_cast<int>(a);
      if (ai >= 4)
         continue;
      auto bi = static_cast<int>(a);
      if (bi >= 4)
         continue;

      auto &state_a = edge_states[ai];
      auto &state_b = edge_states[bi];

      if (state_a.ignore || state_b.ignore)
         continue;

      state_a.ignore = true;
      state_b.tile_count = std::max(state_a.tile_count, state_b.tile_count);
      state_b.closes += state_a.closes;
      state_b.opens += state_a.opens;
      state_b.our = owns_edges(state_a.free, state_a.our, state_b.free, state_b.our);
      state_b.free = state_a.free && state_b.free;
   }

   int score = 0;
   for (auto i = 0; i < 4; ++i) {
      auto &state = edge_states[i];
      if (state.ignore)
         continue;

      auto own_score = state.our ? 1 : (state.free ? 0 : -1);
      auto type_score = state.type == EdgeType::Town ? 4 : 1;
      score += 20 * type_score * own_score * (10 * state.closes + state.opens) * state.tile_count;
   }

   return score;
}

int Game::score_direction(Player player, TileType tile_type, TileMove move, Direction direction) const noexcept {
   const auto tile = g_tiles[tile_type].rotate(move.rotation);
   if (tile.monastery && direction == Direction::Middle) {
      return score_monastery(m_board, move.x, move.y);
   }

   auto edge = make_edge(move.x, move.y, direction);
   if (m_groups.type_of(edge) == EdgeType::Grass) {
      return score_grass(player, edge);
   }
   return score_tile(player, tile, move, direction);
}

std::pair<Direction, int> Game::move_score(Player player, TileType tile_type, TileMove move) const noexcept {
   Direction best_dir{};
   auto best_score = std::numeric_limits<int>::min();
   int total_score{};
   for (auto dir : g_directions) {
      auto score = score_direction(player, tile_type, move, dir);

      if (best_score < score) {
         best_score = score;
         best_dir = dir;
      }
      total_score += score;
   }

   return std::make_pair(best_dir, total_score);
}

}// namespace carcassonne::game