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
   TileType m_current_tile = 1;
   mb::u8 m_rotation = 0;
   int m_selected_tile_x = 0;
   int m_selected_tile_y = 0;
   bool m_selection_ok = false;
   double m_light = 0.0;
   bool m_ligth_down = false;
 public:
   explicit BoardView(IBoard &board);

   void update(double dt);
   void render(const graphics::Context& ctx) const noexcept;
   void scale(double amount);
   void move_cam(double x, double y);
   bool place_tile();
   void rotate_tile();
   void select_tile(double x, double y);
   void set_selected_tile_type(TileType t);
};

}

#endif//MSI_CARCASSONNE_BOARDVIEW_H
