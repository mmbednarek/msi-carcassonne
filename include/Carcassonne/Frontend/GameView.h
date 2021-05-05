#ifndef MSI_CARCASSONNE_GRAPHICS_H
#define MSI_CARCASSONNE_GRAPHICS_H
#include "BoardView.h"
#include "PlaceTileView.h"
#include "PlaceFigureView.h"
#include "FiguresView.h"
#include <Carcassonne/Core.h>
#include <Graphics/Context.h>
#include <Input/Input.h>
#include <random>

namespace carcassonne::frontend {

enum class Status {
   Init,
   Running,
   Quitting,
};

class GameView {
   IGame &m_game;
   std::unique_ptr<IMove> m_move;

   Status m_status = Status::Init;
   Camera m_camera;
   BoardView m_board_view;
   FiguresView m_figures_view;
   PlaceTileView m_place_tile_view;
   PlaceFigureView m_place_figure_view;
   bool m_middle_button_pressed = false;
   int m_last_mouse_x = 0;
   int m_last_mouse_y = 0;


 public:
   explicit GameView(IGame &game);

   void update(double dt);
   void render(const graphics::Context &ctx) const noexcept;

   void on_quit();
   void on_keydown(input::KeyCode key);
   void on_keyup(input::KeyCode key);
   void on_button_down(std::uint8_t button);
   void on_button_up(std::uint8_t button);
   void on_mousemove(int x, int y);
   void on_mouse_wheel(int y);

   void next_move();

   [[nodiscard]] constexpr Status status() const {
      return m_status;
   }
};

}// namespace carcassonne::frontend

#endif//MSI_CARCASSONNE_GRAPHICS_H
