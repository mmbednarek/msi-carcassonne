#ifndef MSI_CARCASSONNE_BOARDVIEW_H
#define MSI_CARCASSONNE_BOARDVIEW_H
#include <Carcassonne/Core.h>
#include <Graphics/Context.h>

namespace carcassonne::frontend {

class BoardView {
   IBoard &m_board;
   double m_scale = 96.0;
   double m_cam_x = 70.0;
   double m_cam_y = 70.0;
 public:
   explicit BoardView(IBoard &board);

   void render(const graphics::Context& ctx) const noexcept;
   void scale(double amount);
   void move_cam(double x, double y);
   void place_random_tile(double x, double y);
   void rotate_tile(double x, double y);
};

}

#endif//MSI_CARCASSONNE_BOARDVIEW_H
