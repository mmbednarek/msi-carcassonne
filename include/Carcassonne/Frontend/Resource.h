#ifndef MSI_CARCASSONNE_RESOURCE_H
#define MSI_CARCASSONNE_RESOURCE_H
#include <Graphics/Surface.h>
#include <Graphics/Texture.h>
#include <array>
#include <mb/int.h>
#include <string_view>

namespace carcassonne::frontend {

enum class TextureResource : mb::u32 {
   YellowFigure,
   Tiles,
   MaxValue
};

struct ResourcePath {
   TextureResource resource;
   std::string_view path;
};

// Singleton resource class
class ResourceManager {
   std::array<graphics::Texture, static_cast<std::size_t>(TextureResource::MaxValue)> m_textures;

   static ResourceManager s_instance;

 public:
   [[nodiscard]] mb::result<mb::empty> load_resources(graphics::Surface &surface);

   [[nodiscard]] constexpr const graphics::Texture &tex(TextureResource t) const {
      return m_textures[static_cast<std::size_t>(t)];
   }

   [[nodiscard]] static constexpr ResourceManager &the() {
      return s_instance;
   }

   [[nodiscard]] static constexpr const graphics::Texture &texture(TextureResource t) {
      return the().tex(t);
   }
};

}// namespace carcassonne::frontend

#endif//MSI_CARCASSONNE_RESOURCE_H
