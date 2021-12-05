#ifndef MSI_CARCASSONNE_GAME_H
#define MSI_CARCASSONNE_GAME_H
#include "Board.h"
#include <Carcassonne/Groups.h>
#include <Carcassonne/IGame.h>
#include <Carcassonne/Move.h>
#include <Carcassonne/ScoreBoard.h>
#include <functional>
#include <mb/int.h>
#include <memory>
#include <memory_resource>
#include <random>

namespace carcassonne::game {

using Towns = std::vector<std::pair<Edge, Edge>>;
using EdgeGroups = Groups<g_edges_max>;
using TileSet = std::vector<TileType>;

constexpr mb::size g_initial_figures_count = 7;
constexpr short g_max_possible_score = 255;

class Game : public IGame {
   Board m_board;
   Player m_current_player = Player::Black;
   mb::size m_player_count;
   mb::size m_move_index = 0;
   std::vector<Figure> m_figures;
   std::vector<NextMoveCallback> m_next_move_callbacks;
   bool m_game_finished = false;
   std::array<mb::u8, 4> m_figure_count{};
   bool m_tour_finished = false;
   FullMove m_performed_move{};
   FullMove m_last_move{};

   Towns m_towns;
   EdgeGroups m_groups;
   ScoreBoard m_scores;
   TileSet m_tile_set;

   std::mt19937_64 m_random_generator;

 public:
   Game(int player_count, mb::u64 seed);
   [[nodiscard]] std::unique_ptr<IGame> clone() const noexcept override;
   [[nodiscard]] const IBoard &board() const noexcept override;
   [[nodiscard]] Player current_player() const noexcept override;
   [[nodiscard]] std::unique_ptr<IMove> new_move(Player p) noexcept override;
   [[nodiscard]] mb::view<Figure> figures() const noexcept override;
   [[nodiscard]] const TileSet &tile_set() const noexcept override;
   [[nodiscard]] bool can_place(TileType tt) const noexcept;
   [[nodiscard]] const ScoreBoard &scores() const noexcept override;
   [[nodiscard]] mb::u8 move_index() const noexcept override;
   [[nodiscard]] bool is_town_field_connected(Edge town, Edge field) noexcept;
   [[nodiscard]] std::vector<Direction> figure_placements(int x, int y) const noexcept override;
   [[nodiscard]] std::pair<Direction, int> move_score(Player player, TileType tile_type, TileMove move, const Parameters &params) const noexcept override;
   [[nodiscard]] bool can_place_figure(int x, int y, Direction d) const override;
   [[nodiscard]] bool can_place_tile_and_figure(int x, int y, mb::u8 rot, TileType tile_type, Direction d) const override;
   void on_next_move(NextMoveCallback callback) noexcept override;
   void start() noexcept override;
   void update(double dt) noexcept override;

   [[nodiscard]] bool is_monastery_completed(int x, int y) noexcept;
   [[nodiscard]] mb::u8 player_figure_count(Player p) const noexcept override;
   void notify_tour_finished(FullMove full_move) noexcept;
   void apply_tile(int x, int y, TileType tt, mb::u8) noexcept;
   void on_structure_completed(Group g);
   void on_monastery_completed(int x, int y, Player player);
   void draw_tiles();
   void add_figure(Figure f);

   [[nodiscard]] constexpr Board &mutable_board() noexcept {
      return m_board;
   }

   [[nodiscard]] constexpr EdgeGroups &mutable_groups() noexcept {
      return m_groups;
   }

   [[nodiscard]] constexpr const EdgeGroups &groups() const noexcept {
      return m_groups;
   }

   [[nodiscard]] constexpr const mb::size &player_count() const noexcept override {
      return m_player_count;
   }
   void board_to_caffe_X(std::vector<float> &output) const override;

 private:
   int score_grass(Player player, Edge edge, const Parameters &params) const noexcept;
   int score_tile(Player player, const Tile &tile, TileMove move, Direction target_direction, const Parameters &params) const noexcept;
   int score_direction(Player player, TileType tile_type, TileMove move, Direction direction, const Parameters &params) const noexcept;
   void set_next_player() noexcept;
   void assign_final_points() noexcept;
};

}// namespace carcassonne::game

#endif//MSI_CARCASSONNE_GAME_H
