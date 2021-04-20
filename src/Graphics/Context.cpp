#include <MSI/Graphics/Context.h>

namespace msi::graphics {

Context::Context(SDL_Renderer *renderer) : m_renderer(renderer) {}

void Context::set_draw_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) const {
   SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
}

void Context::clear() const {
   SDL_RenderClear(m_renderer);
}

void Context::present() const {
   SDL_RenderPresent(m_renderer);
}

void Context::draw_line(int x1, int y1, int x2, int y2) const {
   SDL_RenderDrawLine(m_renderer, x1, y1, x2, y2);
}

}// namespace msi::graphics
