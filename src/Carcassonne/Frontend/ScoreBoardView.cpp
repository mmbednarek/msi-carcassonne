#include <Carcassonne/Frontend/Camera.h>
#include <Carcassonne/Frontend/Object.h>
#include <Carcassonne/Frontend/Resource.h>
#include <Carcassonne/Frontend/ScoreBoardView.h>

namespace carcassonne::frontend {

constexpr auto g_score_board_x = 20;
constexpr auto g_score_board_y = 20;

constexpr auto g_figure_size = 96;

void ScoreBoardView::render(const graphics::Context &ctx) const noexcept {
   auto y = g_score_board_y;
   for (const auto score : m_score_board) {
      ctx.draw(ResourceManager::texture(TextureResource::Figures),
               g_figure_size * static_cast<int>(score.player),
               0,
               g_figure_size,
               g_figure_size,
               g_score_board_x,
               y,
               48,
               48,
               0.0);

      if (auto text = ResourceManager::the().text(std::to_string(score.score)); text != nullptr) {
         ctx.draw(*text, g_score_board_x + 60, y + 10, text->width(), text->height());
      }
      y += 60;
   }
}

}// namespace carcassonne::frontend