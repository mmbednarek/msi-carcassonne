#include <Carcassonne/Graphics/SDLContext.h>

namespace carcassonne::graphics {

void SDLContext::set_draw_color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) const {
   SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
}

void SDLContext::clear() const {
   SDL_RenderClear(m_renderer);
}

void SDLContext::present() const {
   SDL_RenderPresent(m_renderer);
}

void SDLContext::draw_line(int x1, int y1, int x2, int y2) const {
   SDL_RenderDrawLine(m_renderer, x1, y1, x2, y2);
}

void SDLContext::draw(const Texture &tex, int x, int y, int w, int h) const {
   SDL_Rect rect;
   rect.x = x;
   rect.y = y;
   rect.w = w;
   rect.h = h;
   SDL_RenderCopy(m_renderer, tex.raw(), nullptr, &rect);
}

}// namespace carcassonne::graphics
