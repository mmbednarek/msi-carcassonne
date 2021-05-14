#ifndef MSI_CARCASSONNE_GAME_H
#define MSI_CARCASSONNE_GAME_H
#include "Board.h"
#include <Carcassonne/Core.h>
#include <Carcassonne/Groups.h>
#include <Carcassonne/ScoreBoard.h>
#include <mb/int.h>
#include <memory>
#include <random>
#include <memory_resource>

namespace carcassonne::game {

using Towns = std::vector<std::pair<Group, Group>, std::pmr::polymorphic_allocator<std::pair<Group, Group>>>;
using EdgeGroups = Groups<g_edges_max>;

class Game : public IGame {
   Board m_board;
   Player m_current_player = Player::Black;
   mb::u8 m_player_count = 2;
   std::vector<Figure> m_figures;

   Towns m_towns;
   EdgeGroups m_groups;
   ScoreBoard m_scores;

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
   [[nodiscard]] const ScoreBoard &scores() const noexcept override;

   [[nodiscard]] constexpr Board &mutable_board() noexcept {
      return m_board;
   }

   [[nodiscard]] constexpr EdgeGroups &mutable_groups() noexcept {
      return m_groups;
   }

   [[nodiscard]] constexpr const EdgeGroups &groups() const noexcept {
      return m_groups;
   }

   void add_figure(Figure f);

   constexpr void set_next_player() noexcept {
      m_current_player = next_player(m_current_player, m_player_count);
   }

   std::unique_ptr<IMove> new_debug_move(Player p, TileType tt) noexcept;
};

}// namespace carcassonne::game

#endif//MSI_CARCASSONNE_GAME_H
