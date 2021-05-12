#include <Graphics/Surface.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <utility>

namespace graphics {

Surface::~Surface() {
   if (m_renderer != nullptr) {
      SDL_DestroyRenderer(m_renderer);
   }
   if (m_window != nullptr) {
      SDL_DestroyWindow(m_window);
   }

   if (m_renderer != nullptr && m_window != nullptr) {
      SDL_Quit();
   }
}

Surface::Surface(Surface &&other) noexcept : m_window(std::exchange(other.m_window, nullptr)),
                                             m_renderer(std::exchange(other.m_renderer, nullptr)) {}

Surface &Surface::operator=(Surface &&other) noexcept {
   m_window = std::exchange(other.m_window, nullptr);
   m_renderer = std::exchange(other.m_renderer, nullptr);
   return *this;
}

mb::result<Surface> Surface::create(const Config &conf) {
   if (SDL_Init(SDL_INIT_VIDEO) != 0) {
      return mb::error("could not init sdl");
   }

   auto window = SDL_CreateWindow(conf.title.c_str(), -1, -1, conf.width, conf.height, 0);
   if (window == nullptr) {
      return mb::error("could not init window");
   }

   auto renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
   if (renderer == nullptr) {
      return mb::error("could not init renderer");
   }

   SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
   TTF_Init();

   return Surface(renderer, window);
}

}// namespace graphics
