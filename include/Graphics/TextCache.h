#ifndef MSI_CARCASSONNE_TEXTCACHE_H
#define MSI_CARCASSONNE_TEXTCACHE_H
#include "Texture.h"
#include <mb/result.h>
#include <string>
#include <unordered_map>

namespace graphics {

struct CachedTexture {
   Texture texture;
   int reference_count = 0;
};

class TextCache {
   std::unordered_map<std::string, CachedTexture> m_textures;

 public:
   [[nodiscard]] mb::result<Texture &> get_texture(const std::string &text);
   void cache_texture(const std::string &text, Texture tex);
   void reset_references();
   void cleanup();
};

}// namespace graphics

#endif//MSI_CARCASSONNE_TEXTCACHE_H
