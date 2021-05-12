#include <Carcassonne/Frontend/Resource.h>
#include <array>
#include <memory>

namespace carcassonne::frontend {

std::array<ResourcePath, 3> g_paths{
        ResourcePath{TextureResource::YellowFigure, "resource/yellow.png"},
        {TextureResource::Tiles, "resource/tiles.png"},
        {TextureResource::Figures, "resource/figures.png"},
};


ResourceManager ResourceManager::s_instance;

mb::result<mb::empty> ResourceManager::load_resources(graphics::Surface &surface) {
   m_font = std::make_unique<graphics::Font>(MB_TRY(graphics::Font::create("resource/font.ttf", 22)));

   for (const auto &path : g_paths) {
      m_textures[static_cast<std::size_t>(path.resource)] = MB_TRY(graphics::Texture::load(surface, path.path));
   }
   return mb::ok;
}

void ResourceManager::pre_render_hook(const graphics::Surface &surface) {
   m_text_cache.reset_references();

   while (!m_text_render_queue.empty()) {
      auto text = m_text_render_queue.front();
      m_text_render_queue.pop();
      m_text_cache.cache_texture(text, graphics::render_text(surface, *m_font, text, 255, 255, 255, 255));
   }
}

graphics::Texture *ResourceManager::text(const std::string &text) {
   auto cache_res = m_text_cache.get_texture(text);
   if (cache_res.ok()) {
      return &cache_res.unwrap();
   }
   m_text_render_queue.push(text);
   return nullptr;
}

}// namespace carcassonne::frontend