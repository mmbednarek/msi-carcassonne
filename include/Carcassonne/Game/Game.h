#ifndef MSI_CARCASSONNE_GAME_H
#define MSI_CARCASSONNE_GAME_H
#include "Board.h"
#include <Carcassonne/Core.h>
#include <Carcassonne/Groups.h>
#include <mb/int.h>
#include <memory>
#include <random>
#include <unordered_set>

namespace carcassonne::game {

using EdgeGroups = Groups<2 * g_board_width * g_board_height + g_board_width + g_board_height>;

class Game : public IGame {
   Board m_board;
   Player m_current_player = Player::Black;
   mb::u8 m_player_count = 2;
   std::vector<Figure> m_figures;

   EdgeGroups m_groups;

   std::random_device m_random_device;
   std::mt19937_64 m_random_generator;
   std::uniform_int_distribution<unsigned long long> m_random_distribution;

 public:
   Game();

   [[nodiscard]] const IBoard &board() const noexcept override;
   [[nodiscard]] Player current_player() const noexcept override;
   [[nodiscard]] std::unique_ptr<IMove> new_move(Player p) noexcept override;
   [[nodiscard]] mb::view<Figure> figures() const noexcept override;

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
};

}// namespace carcassonne::game

#endif//MSI_CARCASSONNE_GAME_H
