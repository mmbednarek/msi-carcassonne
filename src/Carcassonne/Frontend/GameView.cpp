#include <Carcassonne/Frontend/GameView.h>
#include <Carcassonne/Frontend/Resource.h>
#include <Carcassonne/Game/Game.h>
#include <mb/int.h>
#include <random>

namespace carcassonne::frontend {

GameView::GameView(IGame &game) : m_game(game), m_board_view(game.board()) {
}

void GameView::render(const graphics::Context &ctx) const noexcept {
   ctx.set_draw_color(39, 79, 136, 255);
   ctx.clear();

   m_board_view.render(ctx);

   ctx.present();
}

void GameView::update(double dt) {
   m_board_view.update(dt);
}

void GameView::on_quit() {
   m_status = Status::Quitting;
}

void GameView::on_keydown(input::KeyCode key) {
}

void GameView::on_keyup(input::KeyCode key) {
}

void GameView::on_button_down(std::uint8_t button) {
   switch (button) {
   case SDL_BUTTON_LEFT:
      if (m_board_view.place_tile()) {
         m_board_view.set_selected_tile_type((rand() % 24) + 1);
      }
      break;
   case SDL_BUTTON_MIDDLE:
      m_middle_button_pressed = true;
      break;
   case SDL_BUTTON_RIGHT:
      m_board_view.rotate_tile();
      break;
   default: break;
   }
}

void GameView::on_button_up(std::uint8_t button) {
   if (button == SDL_BUTTON_MIDDLE) {
      m_middle_button_pressed = false;
   }
}

void GameView::on_mousemove(int x, int y) {
   if (m_middle_button_pressed) {
      m_board_view.move_cam(m_last_mouse_x - x, m_last_mouse_y - y);
   } else {
      m_board_view.select_tile(x, y);
   }

   m_last_mouse_x = x;
   m_last_mouse_y = y;
}

void GameView::on_mouse_wheel(int y) {
   m_board_view.move_cam(m_last_mouse_x, m_last_mouse_y);
   m_board_view.scale(8*y);
   m_board_view.move_cam(-m_last_mouse_x, -m_last_mouse_y);
}

}// namespace carcassonne::frontend