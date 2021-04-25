#ifndef MSI_CARCASSONNE_TEXTURE_H
#define MSI_CARCASSONNE_TEXTURE_H
#include <SDL2/SDL.h>
#include <mb/result.h>
#include <string_view>

namespace graphics {

class Surface;

class Texture {
   SDL_Surface *m_surface = nullptr;
   SDL_Texture *m_texture = nullptr;

 public:
   Texture() = default;
   constexpr Texture(SDL_Surface *surface, SDL_Texture *texture) : m_surface(surface), m_texture(texture) {}

   ~Texture();
   Texture(const Texture &) = delete;
   Texture &operator=(const Texture &) = delete;
   Texture(Texture &&other) noexcept;
   Texture &operator=(Texture &&other) noexcept;

   [[nodiscard]] static mb::result<Texture> load(Surface &surface, std::string_view path);

   [[nodiscard]] constexpr SDL_Texture *raw() const {
      return m_texture;
   };
};

}// namespace graphics

#endif//MSI_CARCASSONNE_TEXTURE_H
