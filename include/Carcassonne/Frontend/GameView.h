#ifndef MSI_CARCASSONNE_GRAPHICS_H
#define MSI_CARCASSONNE_GRAPHICS_H
#include <Carcassonne/Core.h>
#include <Graphics/Context.h>
#include <Input/Input.h>

namespace carcassonne::frontend {

enum class Status {
   Init,
   Running,
   Quitting,
};

class GameView {
   IGame &m_game;
   Status m_status = Status::Init;

 public:
   explicit GameView(IGame &game);

   void render(const graphics::Context &ctx) const noexcept;

   void on_quit();
   void on_keydown(input::KeyCode key);
   void on_keyup(input::KeyCode key);
   void on_button_down(std::uint8_t button);
   void on_button_up(std::uint8_t button);
   void on_mousemove(int x, int y);

   [[nodiscard]] constexpr Status status() const {
      return m_status;
   }
};

}// namespace carcassonne::frontend

#endif//MSI_CARCASSONNE_GRAPHICS_H
