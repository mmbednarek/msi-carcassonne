#ifndef MSI_CARCASSONNE_PLACETILEVIEW_H
#define MSI_CARCASSONNE_PLACETILEVIEW_H
#include <Carcassonne/IGame.h>
#include <Carcassonne/Frontend/Camera.h>
#include <Graphics/Context.h>

namespace carcassonne::frontend {

class PlaceTileView {
   const IBoard &m_board;
   const Camera &m_camera;

   std::unique_ptr<IMove> &m_move;
   mb::u8 m_rotation = 0;

   int m_tile_x = 0;
   int m_tile_y = 0;

   bool m_selection_ok = false;

   double m_flicker = 0.0;
   bool m_flicker_raising = false;

 public:
   PlaceTileView(const IBoard &board, const Camera &camera, std::unique_ptr<IMove> &move);

   void render(const graphics::Context &ctx) const;
   void update(double dt);

   bool place_tile();
   void rotate_tile();
   void select_tile(double x, double y);
};

}// namespace carcassonne::frontend

#endif//MSI_CARCASSONNE_PLACETILEVIEW_H
