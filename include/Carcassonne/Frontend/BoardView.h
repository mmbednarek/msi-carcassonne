#ifndef MSI_CARCASSONNE_BOARDVIEW_H
#define MSI_CARCASSONNE_BOARDVIEW_H
#include <Carcassonne/IGame.h>
#include <Graphics/Context.h>
#include "Camera.h"

namespace carcassonne::frontend {

class BoardView {
   const IBoard &m_board;
   const Camera &m_camera;

 public:
   BoardView(const IBoard &board, const Camera &camera);

   void update(double dt);
   void render(const graphics::Context &ctx) const noexcept;
};

}// namespace carcassonne::frontend

#endif//MSI_CARCASSONNE_BOARDVIEW_H
