#include <Graphics/Font.h>
#include <utility>
#include <filesystem>
#include <sstream>
#include <fmt/core.h>

namespace graphics {

Font::~Font() {
   TTF_CloseFont(m_font);
}

Font::Font(Font &&other) noexcept : m_font(std::exchange(other.m_font, nullptr)) {}

Font &Font::operator=(Font &&other) noexcept {
   m_font = std::exchange(other.m_font, nullptr);
   return *this;
}

TTF_Font *Font::raw() const {
   return m_font;
}

mb::result<Font> Font::create(const std::string &path, int size) {
   auto font = TTF_OpenFont(path.c_str(), size);
   if (font == nullptr) {
      return mb::error(fmt::format("could not create font: {}", TTF_GetError()));
   }
   TTF_SetFontHinting(font, TTF_HINTING_MONO);
   return Font(font);
}

}// namespace graphics