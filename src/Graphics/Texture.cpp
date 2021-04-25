#include <Carcassonne/Graphics/Surface.h>
#include <Carcassonne/Graphics/Texture.h>
#include <SDL2/SDL_image.h>
#include <fmt/core.h>

namespace carcassonne::graphics {

Texture::~Texture() {
   if (m_texture != nullptr) {
      SDL_DestroyTexture(m_texture);
   }
   if (m_surface != nullptr) {
      SDL_FreeSurface(m_surface);
   }
}

mb::result<Texture> Texture::load(Surface &surface, std::string_view path) {
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

}// namespace carcassonne::graphics