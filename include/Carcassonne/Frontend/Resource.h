#ifndef MSI_CARCASSONNE_RESOURCE_H
#define MSI_CARCASSONNE_RESOURCE_H
#include <Graphics/Surface.h>
#include <Graphics/Texture.h>
#include <Graphics/TextCache.h>
#include <array>
#include <mb/int.h>
#include <string_view>
#include <queue>

namespace carcassonne::frontend {

enum class TextureResource : mb::u32 {
   YellowFigure,
   Tiles,
   Figures,
   MaxValue
};

struct ResourcePath {
   TextureResource resource;
   std::string_view path;
};

// Singleton resource class
class ResourceManager {
   std::array<graphics::Texture, static_cast<std::size_t>(TextureResource::MaxValue)> m_textures;
   std::unique_ptr<graphics::Font> m_font;
   graphics::TextCache m_text_cache;
   std::queue<std::string> m_text_render_queue;

   static ResourceManager s_instance;

 public:
   [[nodiscard]] mb::result<mb::empty> load_resources(graphics::Surface &surface);

   [[nodiscard]] constexpr const graphics::Texture &tex(TextureResource t) const {
      return m_textures[static_cast<std::size_t>(t)];
   }

   [[nodiscard]] graphics::Texture *text(const std::string &text);

   void pre_render_hook(const graphics::Surface &surface);

   inline void post_render_hook() {
      m_text_cache.cleanup();
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
