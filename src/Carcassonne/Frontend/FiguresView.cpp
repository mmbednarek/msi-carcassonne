#include <Carcassonne/Frontend/FiguresView.h>
#include <Carcassonne/Frontend/Object.h>
#include <Carcassonne/Frontend/Resource.h>

namespace carcassonne::frontend {

FiguresView::FiguresView(const IGame &game, const Camera &camera) : m_game(game), m_camera(camera) {}

void FiguresView::render(const graphics::Context &ctx) const noexcept {
   const auto &figures = m_game.figures();
   for (const auto figure : figures) {
      render_figure(ctx, m_camera, figure.x, figure.y, figure.player);
   }
}

}// namespace carcassonne::frontend