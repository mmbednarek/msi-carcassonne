#include <MSI/Graphics/Surface.h>
#include <SDL2/SDL.h>
#include <stdexcept>
#include <utility>

namespace msi::graphics {

Surface::Surface(const Config &conf) {
   if (SDL_Init(SDL_INIT_VIDEO) != 0) {
      throw std::runtime_error("could not init sdl");
   }
   if (SDL_CreateWindowAndRenderer(conf.width, conf.height, 0, &m_window, &m_renderer) != 0) {
      throw std::runtime_error("could not init window or renderer");
   }
}

Surface::~Surface() {
   if (m_renderer != nullptr) {
      SDL_DestroyRenderer(m_renderer);
   }
   if (m_window != nullptr) {
      SDL_DestroyWindow(m_window);
   }
   SDL_Quit();
}

Surface::Surface(Surface &&other) noexcept : m_window(std::exchange(other.m_window, nullptr)),
                                             m_renderer(std::exchange(other.m_renderer, nullptr)) {}

Surface &Surface::operator=(Surface &&other) noexcept {
   m_window = std::exchange(other.m_window, nullptr);
   m_renderer = std::exchange(other.m_renderer, nullptr);
   return *this;
}

void Surface::on_event(SDL_Event &event) noexcept {
   if (event.type == SDL_QUIT) {
      m_running = false;
      return;
   }
}

}// namespace msi::graphics
