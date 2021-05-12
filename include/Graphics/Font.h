#ifndef MSI_CARCASSONNE_FONT_H
#define MSI_CARCASSONNE_FONT_H
#include <SDL2/SDL_ttf.h>
#include <mb/result.h>

namespace graphics {

class Font {
   TTF_Font *m_font;

 public:
   constexpr explicit Font(TTF_Font *font) : m_font(font) {}

   ~Font();
   Font(const Font &) = delete;
   Font &operator=(const Font &) = delete;
   Font(Font &&other) noexcept;
   Font &operator=(Font &&other) noexcept;

   static mb::result<Font> create(const std::string &path, int size);

   [[nodiscard]] TTF_Font *raw() const;
};

}

#endif//MSI_CARCASSONNE_FONT_H
