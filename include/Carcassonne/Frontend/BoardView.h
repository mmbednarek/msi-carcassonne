#ifndef MSI_CARCASSONNE_BOARDVIEW_H
#define MSI_CARCASSONNE_BOARDVIEW_H
#include <Carcassonne/Core.h>
#include <Graphics/Context.h>
#include "Camera.h"

namespace carcassonne::frontend {

class BoardView {
   const IBoard &m_board;
   const Camera &m_camera;
   TileType m_current_tile = 1;
   mb::u8 m_rotation = 0;
   int m_selected_tile_x = 0;
   int m_selected_tile_y = 0;
   bool m_selection_ok = false;
   double m_light = 0.0;
   bool m_light_down = false;

 public:
   BoardView(const IBoard &board, const Camera &camera);

   void update(double dt);
   void render(const graphics::Context &ctx) const noexcept;
};

}// namespace carcassonne::frontend

#endif//MSI_CARCASSONNE_BOARDVIEW_H
