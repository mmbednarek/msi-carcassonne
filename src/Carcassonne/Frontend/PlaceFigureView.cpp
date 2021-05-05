#include <Carcassonne/Frontend/Object.h>
#include <Carcassonne/Frontend/PlaceFigureView.h>
#include <Carcassonne/Frontend/Resource.h>
#include <cmath>

namespace carcassonne::frontend {

constexpr double distance(double ax, double ay, double bx, double by) {
   return std::sqrt(std::pow(ax - bx, 2.0) + std::pow(ay - by, 2.0));
}

PlaceFigureView::PlaceFigureView(const Camera &camera, std::unique_ptr<IMove> &move) : m_camera(camera), m_move(move) {}

void PlaceFigureView::render(const graphics::Context &ctx) const noexcept {
   double px, py;
   std::tie(px, py) = direction_position(m_move->position(), m_direction);
   render_figure(ctx, m_camera, px, py, m_move->player());
}

void PlaceFigureView::update_direction(int x, int y) {
   auto mouse_x = m_camera.reverse_x(x);
   auto mouse_y = m_camera.reverse_y(y);
   double min_distance = std::numeric_limits<double>::infinity();
   const auto tile_position = m_move->position();

   for (const auto direction : g_directions) {
      if (!m_move->is_free(direction))
         continue;

      double px, py;
      std::tie(px, py) = direction_position(tile_position, direction);
      auto dist = distance(px, py, mouse_x, mouse_y);
      if (dist < min_distance) {
         min_distance = dist;
         m_direction = direction;
      }
   }
}

void PlaceFigureView::place_figure() {
   m_move->place_figure(m_direction);
}

void PlaceFigureView::ignore_figure() {
   m_move->ignore_figure();
}

}// namespace carcassonne::frontend