#ifndef MSI_CARCASSONNE_TEXTURE_H
#define MSI_CARCASSONNE_TEXTURE_H
#include "Font.h"
#include <SDL2/SDL.h>
#include <mb/int.h>
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

   [[nodiscard]] int width() const;
   [[nodiscard]] int height() const;

   [[nodiscard]] static mb::result<Texture> load(const Surface &surface, std::string_view path);

   [[nodiscard]] constexpr SDL_Texture *raw() const {
      return m_texture;
   };
};

Texture render_text(const Surface &surface, const Font &font, const std::string &text, mb::u8 r, mb::u8 g, mb::u8 b, mb::u8 a);

}// namespace graphics

#endif//MSI_CARCASSONNE_TEXTURE_H
