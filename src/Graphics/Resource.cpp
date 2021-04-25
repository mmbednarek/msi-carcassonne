#include <Carcassonne/Graphics/Resource.h>
#include <array>

namespace carcassonne::graphics {

std::array<ResourcePath, 1> g_paths{
        {TextureResource::YellowFigure, "resource/yellow.png"},
};

ResourceManager g_resource_manager_instance;

mb::result<mb::empty> ResourceManager::load_resources(Surface &surface) {
   for (const auto &path : g_paths) {
      m_textures[static_cast<std::size_t>(path.resource)] = MB_TRY(Texture::load(surface, path.path));
   }
   return mb::ok;
}

}// namespace carcassonne::graphics