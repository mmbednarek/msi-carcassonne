#ifndef MSI_CARCASSONNE_FIGURESVIEW_H
#define MSI_CARCASSONNE_FIGURESVIEW_H
#include "Camera.h"
#include <Carcassonne/Core.h>
#include <Graphics/Context.h>

namespace carcassonne::frontend {

class FiguresView {
   const IGame &m_game;
   const Camera &m_camera;

 public:
   explicit FiguresView(const IGame &game, const Camera &camera);

   void render(const graphics::Context &ctx) const noexcept;
};

}// namespace carcassonne::frontend

#endif//MSI_CARCASSONNE_FIGURESVIEW_H
