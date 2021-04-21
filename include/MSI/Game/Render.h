#ifndef MSI_CARCASSONNE_GRAPHICS_H
#define MSI_CARCASSONNE_GRAPHICS_H
#include <MSI/Graphics/Context.h>

namespace msi::game {

class Game;

class Render {
   Game &m_game;

 public:
   explicit Render(Game &game);

   void render(const graphics::IContext &ctx) const noexcept;
};

}// namespace msi::game

#endif//MSI_CARCASSONNE_GRAPHICS_H
