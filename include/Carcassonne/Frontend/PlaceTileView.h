#ifndef MSI_CARCASSONNE_PLACETILEVIEW_H
#define MSI_CARCASSONNE_PLACETILEVIEW_H
#include <Carcassonne/Core.h>
#include <Carcassonne/Frontend/Camera.h>
#include <Graphics/Context.h>

namespace carcassonne::frontend {

class PlaceTileView {
   IBoard &m_board;
   const Camera &m_camera;

   TileType m_tile_type = 0;
   mb::u8 m_rotation = 0;

   int m_tile_x = 0;
   int m_tile_y = 0;

   bool m_selection_ok = false;

   double m_flicker = 0.0;
   bool m_flicker_raising = false;

 public:
   PlaceTileView(IBoard &board, const Camera &camera);

   void render(const graphics::Context &ctx) const;
   void update(double dt);

   bool place_tile();
   void rotate_tile();
   void select_tile(double x, double y);
   void set_tile_type(TileType t);
};

}// namespace carcassonne::frontend

#endif//MSI_CARCASSONNE_PLACETILEVIEW_H
