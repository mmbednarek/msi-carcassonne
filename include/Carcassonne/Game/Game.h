#ifndef MSI_CARCASSONNE_GAME_H
#define MSI_CARCASSONNE_GAME_H
#include "Board.h"
#include <Carcassonne/Core.h>
#include <mb/int.h>

namespace carcassonne::game {

class Game : public IGame {
   Position m_block_pos{20.0f, 20.0f};
   Board m_board;

 public:
   void update();
   IBoard &board() noexcept override;

   [[nodiscard]] const Position &block_pos() const noexcept override;
   void set_block_pos(mb::f64 x, mb::f64 y) noexcept override;
};

}// namespace carcassonne::game

#endif//MSI_CARCASSONNE_GAME_H
