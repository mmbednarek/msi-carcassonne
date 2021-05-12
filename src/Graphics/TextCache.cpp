#include <Graphics/TextCache.h>
#include <algorithm>

namespace graphics {

mb::result<Texture &> TextCache::get_texture(const std::string &view) {
   auto it = m_textures.find(view);
   if (it == m_textures.end()) {
      return mb::error("cache miss");
   }
   ++it->second.reference_count;
   return it->second.texture;
}

void TextCache::cache_texture(const std::string &text, Texture tex) {
   m_textures[text] = CachedTexture{
      .texture = std::move(tex),
      .reference_count = 1,
   };
}

void TextCache::reset_references() {
   for (auto &cached_texture : m_textures) {
      cached_texture.second.reference_count = 0;
   }
}

void TextCache::cleanup() {
   std::erase_if(m_textures, [](const auto &cached_texture) { return cached_texture.second.reference_count == 0; } );
}

}// namespace graphics