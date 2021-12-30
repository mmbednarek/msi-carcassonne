#ifndef CARCASSONNE_GPU_GAME_H
#define CARCASSONNE_GPU_GAME_H
#ifdef __CUDACC__
#define DEVHOST __device__ __host__
#else
#define DEVHOST
#endif

#include "Board.h"
#include "Move.h"
#include <Carcassonne/Groups.h>
#include <Carcassonne/ScoreBoard.h>
#include <Carcassonne/Direction.h>
#include <Carcassonne/Edge.h>
#include <Carcassonne/Player.h>
#include <Carcassonne/Parameters.h>
#include <mb/int.h>
// #include <memory_resource>
#include <random>
#include <functional>
#include <thrust/fill.h>
#include <thrust/device_vector.h>
#include <ecuda/ptr/unique_ptr.hpp>

namespace carcassonne::game_gpu {

class Game;

using Towns = std::vector<std::pair<Edge, Edge>>;
using EdgeGroups = Groups<g_edges_max>;
using TileSet = std::vector<TileType>;
using NextMoveCallback = std::function<void(Game &, Player, FullMove)>;

DEVHOST constexpr mb::size g_initial_figures_count = 7;
DEVHOST constexpr short g_max_possible_score = 255;

struct Figure {
   Player player;
   double x, y;
   int tile_x, tile_y;
   Edge edge;
   Direction dir;
};

class Game {
   Board m_board;
   Player m_current_player = Player::Black;
   mb::size m_player_count;
   mb::size m_move_index = 0;
   std::vector<Figure> m_figures;
   std::vector<NextMoveCallback> m_next_move_callbacks;
   bool m_game_finished = false;
   std::array<mb::u8, 4> m_figure_count{};
   bool m_tour_finished = false;
   FullMove m_performed_move;
   FullMove m_last_move;

   Towns m_towns;
   EdgeGroups m_groups;
   ScoreBoard m_scores;
   TileSet m_tile_set;

   std::mt19937_64 m_random_generator;

 public:
   Game(int player_count, mb::u64 seed);
   Game(const Game&) = default;
   [[nodiscard]] ecuda::unique_ptr<Game> clone() const noexcept;
   [[nodiscard]] const Board &board() const noexcept;
   [[nodiscard]] const Player &current_player() const noexcept;
   [[nodiscard]] Move new_move(Player p) noexcept;
   [[nodiscard]] const std::vector<Figure> &figures() const noexcept;
   [[nodiscard]] const TileSet &tile_set() const noexcept;
   [[nodiscard]] bool can_place(TileType tt) const noexcept;
   [[nodiscard]] const ScoreBoard &scores() const noexcept;
   [[nodiscard]] mb::u8 move_index() const noexcept;
   [[nodiscard]] bool is_town_field_connected(Edge town, Edge field) noexcept;
   [[nodiscard]] std::vector<Direction> figure_placements(int x, int y) const noexcept;
   [[nodiscard]] std::pair<Direction, int> move_score(Player player, TileType tile_type, TileMove move, const Parameters &params) const noexcept;
   [[nodiscard]] bool can_place_figure(int x, int y, Direction d) const;
   [[nodiscard]] bool can_place_tile_and_figure(int x, int y, mb::u8 rot, TileType tile_type, Direction d) const;
   void on_next_move(NextMoveCallback callback) noexcept;
   void start() noexcept;
   void update(double dt) noexcept;

   [[nodiscard]] bool is_monastery_completed(int x, int y) noexcept;
   [[nodiscard]] mb::u8 player_figure_count(Player p) const noexcept;
   void notify_tour_finished(FullMove full_move) noexcept;
   void apply_tile(int x, int y, TileType tt, mb::u8) noexcept;
   void on_structure_completed(Group g);
   void on_monastery_completed(int x, int y, Player player);
   void draw_tiles();
   void add_figure(Figure f);

   [[nodiscard]] DEVHOST constexpr Board &mutable_board() noexcept {
      return m_board;
   }

   [[nodiscard]] DEVHOST constexpr EdgeGroups &mutable_groups() noexcept {
      return m_groups;
   }

   [[nodiscard]] DEVHOST constexpr const EdgeGroups &groups() const noexcept {
      return m_groups;
   }

   [[nodiscard]] /*constexpr*/ DEVHOST const mb::size &player_count() const noexcept {
      return m_player_count;
   }
   DEVHOST void board_to_caffe_X(std::vector<float> &output) const;

   [[nodiscard]] inline MoveRange moves(TileType tile_type = 0) const {
      if (tile_type == 0) {
         tile_type = tile_set()[move_index()];
      }
      return MoveRange{
              .from = MoveIter(board(), tile_type),
              .to = MoveIter(board(), tile_type, board().min_x() - 1, board().max_y() + 1, 0),
      };
   }

 private:
   int score_grass(Player player, Edge edge, const Parameters &params) const noexcept;
   int score_tile(Player player, const Tile &tile, TileMove move, Direction target_direction, const Parameters &params) const noexcept;
   int score_direction(Player player, TileType tile_type, TileMove move, Direction direction, const Parameters &params) const noexcept;
   void set_next_player() noexcept;
   void assign_final_points() noexcept;
};

}// namespace carcassonne::game

#endif//CARCASSONNE_GPU_GAME_H
