#include <Graphics/Context.h>

namespace graphics {

void Context::set_draw_color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) const {
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

void Context::draw(const Texture &tex, int x, int y, int w, int h) const {
   SDL_Rect rect;
   rect.x = x;
   rect.y = y;
   rect.w = w;
   rect.h = h;
   SDL_RenderCopy(m_renderer, tex.raw(), nullptr, &rect);
}

void Context::draw(const Texture &tex, int tx, int ty, int tw, int th, int x, int y, int w, int h, double angle) const {
   SDL_Rect src_rect;
   src_rect.x = tx;
   src_rect.y = ty;
   src_rect.w = tw;
   src_rect.h = th;
   SDL_Rect dst_rect;
   dst_rect.x = x;
   dst_rect.y = y;
   dst_rect.w = w;
   dst_rect.h = h;
   SDL_RenderCopyEx(m_renderer, tex.raw(), &src_rect, &dst_rect, angle, nullptr, SDL_FLIP_NONE);
}

}// namespace graphics
