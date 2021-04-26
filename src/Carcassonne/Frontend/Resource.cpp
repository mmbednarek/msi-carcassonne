#include <Carcassonne/Frontend/Resource.h>
#include <array>

namespace carcassonne::frontend {

std::array<ResourcePath, 2> g_paths{
        ResourcePath{TextureResource::YellowFigure, "resource/yellow.png"},
        {TextureResource::Tiles, "resource/tiles.png"},
};

ResourceManager g_resource_manager_instance;

mb::result<mb::empty> ResourceManager::load_resources(graphics::Surface &surface) {
   for (const auto &path : g_paths) {
      m_textures[static_cast<std::size_t>(path.resource)] = MB_TRY(graphics::Texture::load(surface, path.path));
   }
   return mb::ok;
}

}// namespace carcassonne::frontend