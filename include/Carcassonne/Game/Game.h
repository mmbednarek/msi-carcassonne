#ifndef MSI_CARCASSONNE_GAME_H
#define MSI_CARCASSONNE_GAME_H
#include <Carcassonne/Core.h>
#include <mb/int.h>

namespace carcassonne::game {

class Game : public IGame {
   Position m_block_pos{20.0f, 20.0f};

 public:
   void update();

    [[nodiscard]] const Position &block_pos() const noexcept override;
    void set_block_pos(mb::f64 x, mb::f64 y) noexcept override;
};

}// namespace carcassonne::game

#endif//MSI_CARCASSONNE_GAME_H
