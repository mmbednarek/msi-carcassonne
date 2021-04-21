#ifndef MSI_CARCASSONNE_RESOURCE_H
#define MSI_CARCASSONNE_RESOURCE_H
#include <MSI/Graphics/Surface.h>
#include <MSI/Graphics/Texture.h>
#include <array>
#include <mb/int.h>
#include <string_view>

namespace msi::graphics {

enum class TextureResource : mb::u32 {
   YellowFigure,
   MaxValue
};

struct ResourcePath {
   TextureResource resource;
   std::string_view path;
};

class ResourceManager;
extern ResourceManager g_resource_manager_instance;

// Singleton resource class
class ResourceManager {
   std::array<Texture, static_cast<std::size_t>(TextureResource::MaxValue)> m_textures;

 public:
   [[nodiscard]] mb::result<mb::empty> load_resources(graphics::Surface &surface);

   [[nodiscard]] constexpr const Texture &tex(TextureResource t) const {
      return m_textures[static_cast<std::size_t>(t)];
   }

   [[nodiscard]] static constexpr ResourceManager &the() {
      return g_resource_manager_instance;
   }

   [[nodiscard]] static constexpr const Texture &texture(TextureResource t) {
      return the().tex(t);
   }
};

}// namespace msi::graphics

#endif//MSI_CARCASSONNE_RESOURCE_H
