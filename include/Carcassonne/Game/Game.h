#ifndef MSI_CARCASSONNE_GAME_H
#define MSI_CARCASSONNE_GAME_H
#include "Board.h"
#include <Carcassonne/Core.h>
#include <Carcassonne/Groups.h>
#include <Carcassonne/ScoreBoard.h>
#include <mb/int.h>
#include <memory>
#include <memory_resource>
#include <random>

namespace carcassonne::game {

using Towns = std::vector<std::pair<Group, Group>>;
using EdgeGroups = Groups<g_edges_max>;
using TileSet = std::vector<TileType>;
using TileSets = std::vector<TileSet>;

class Game : public IGame {
   Board m_board;
   Player m_current_player = Player::Black;
   mb::u8 m_player_count = 2;
   mb::u8 m_move = 0;
   std::vector<Figure> m_figures;

   Towns m_towns;
   EdgeGroups m_groups;
   ScoreBoard m_scores;
   TileSet m_tiles_to_draw;
   TileSets m_tile_sets;

   std::random_device m_random_device;
   std::mt19937_64 m_random_generator;
   std::uniform_int_distribution<unsigned long long> m_random_distribution;

 public:
   Game();

   [[nodiscard]] const IBoard &board() const noexcept override;
   [[nodiscard]] Player current_player() const noexcept override;
   [[nodiscard]] std::unique_ptr<IMove> new_move(Player p) noexcept override;
   [[nodiscard]] mb::view<Figure> figures() const noexcept override;
   void apply_tile(int x, int y, TileType tt, mb::u8) noexcept;
   void on_structure_completed(Group g);
   void on_monastery_completed(int x, int y, Player player);
   void draw_tiles();
   [[nodiscard]] const ScoreBoard &scores() const noexcept override;
   [[nodiscard]] bool is_town_field_connected(Edge town, Edge field) const noexcept;

   [[nodiscard]] constexpr Board &mutable_board() noexcept {
      return m_board;
   }

   [[nodiscard]] constexpr EdgeGroups &mutable_groups() noexcept {
      return m_groups;
   }

   [[nodiscard]] constexpr const EdgeGroups &groups() const noexcept {
      return m_groups;
   }

   [[nodiscard]] bool is_monastery_completed(int x, int y) noexcept;

   void add_figure(Figure f);

   constexpr void set_next_player() noexcept {
      m_current_player = next_player(m_current_player, m_player_count);
   }
};

}// namespace carcassonne::game

#endif//MSI_CARCASSONNE_GAME_H
