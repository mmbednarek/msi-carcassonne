#ifndef MSI_CARCASSONNE_PLACEFIGUREVIEW_H
#define MSI_CARCASSONNE_PLACEFIGUREVIEW_H
#include <Carcassonne/Core.h>
#include <Graphics/Context.h>
#include "Camera.h"

namespace carcassonne::frontend {

class PlaceFigureView {
   std::unique_ptr<IMove> &m_move;
   const Camera &m_camera;
   Direction m_direction = Direction::North;

 public:
   explicit PlaceFigureView(const Camera &camera, std::unique_ptr<IMove> &move);

   void render(const graphics::Context &ctx) const noexcept;

   void update_direction(int x, int y);
   void place_figure();
   void ignore_figure();
};

}

#endif//MSI_CARCASSONNE_PLACEFIGUREVIEW_H
