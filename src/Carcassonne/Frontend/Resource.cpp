#include <Carcassonne/Frontend/Resource.h>
#include <array>

namespace carcassonne::frontend {

std::array<ResourcePath, 3> g_paths{
        ResourcePath{TextureResource::YellowFigure, "resource/yellow.png"},
        {TextureResource::Tiles, "resource/tiles.png"},
        {TextureResource::Figures, "resource/figures.png"},
};


ResourceManager ResourceManager::s_instance;

mb::result<mb::empty> ResourceManager::load_resources(graphics::Surface &surface) {
   for (const auto &path : g_paths) {
      m_textures[static_cast<std::size_t>(path.resource)] = MB_TRY(graphics::Texture::load(surface, path.path));
   }
   return mb::ok;
}

}// namespace carcassonne::frontend