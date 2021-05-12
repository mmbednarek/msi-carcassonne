#include <Graphics/Surface.h>
#include <Graphics/Texture.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <fmt/core.h>

namespace graphics {

Texture::~Texture() {
   if (m_texture != nullptr) {
      SDL_DestroyTexture(m_texture);
   }
   if (m_surface != nullptr) {
      SDL_FreeSurface(m_surface);
   }
}

mb::result<Texture> Texture::load(const Surface &surface, std::string_view path) {
   auto image = IMG_Load(path.data());
   if (image == nullptr) {
      return mb::error(fmt::format("could not load image \"{}\"", path));
   }

   auto texture = SDL_CreateTextureFromSurface(surface.raw(), image);
   if (texture == nullptr) {
      return mb::error(fmt::format("could not create texture \"{}\"", path));
   }

   return Texture(image, texture);
}

Texture::Texture(Texture &&other) noexcept : m_surface(std::exchange(other.m_surface, nullptr)),
                                             m_texture(std::exchange(other.m_texture, nullptr)) {}

Texture &Texture::operator=(Texture &&other) noexcept {
   m_surface = std::exchange(other.m_surface, nullptr);
   m_texture = std::exchange(other.m_texture, nullptr);
   return *this;
}
int Texture::width() const {
   return m_surface->w;
}

int Texture::height() const {
   return m_surface->h;
}

Texture render_text(const Surface &surface, const Font &font, const std::string &text, mb::u8 r, mb::u8 g, mb::u8 b, mb::u8 a) {
   SDL_Color color{
           .r = r,
           .g = g,
           .b = b,
           .a = a,
   };
   auto *texture_surface = TTF_RenderText_Solid(font.raw(), text.c_str(), color);
   auto *texture = SDL_CreateTextureFromSurface(surface.raw(), texture_surface);
   return Texture(texture_surface, texture);
}

}// namespace graphics