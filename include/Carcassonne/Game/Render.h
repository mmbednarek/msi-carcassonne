#ifndef MSI_CARCASSONNE_GRAPHICS_H
#define MSI_CARCASSONNE_GRAPHICS_H
#include <Carcassonne/Graphics/Context.h>

namespace carcassonne::game {

class Game;

class Render {
   Game &m_game;

 public:
   explicit Render(Game &game);

   void render(const graphics::IContext &ctx) const noexcept;
};

}// namespace carcassonne::game

#endif//MSI_CARCASSONNE_GRAPHICS_H
