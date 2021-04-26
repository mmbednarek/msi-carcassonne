#include <Carcassonne/Frontend/GameView.h>
#include <Carcassonne/Frontend/Resource.h>
#include <Carcassonne/Game/Game.h>
#include <mb/int.h>
#include <random>

namespace carcassonne::frontend {

std::array<std::tuple<int, int, double>, 100> g_board{};

GameView::GameView(IGame &game) : m_game(game), m_board_view(game.board()) {
   std::random_device rd;
   std::mt19937_64 gen(rd());
   std::uniform_int_distribution<int> dis;

   for (int y = 0; y < 10; ++y) {
      for (int x = 0; x < 10; ++x) {
         g_board[x + y * 10] = std::make_tuple(dis(gen) % 4, dis(gen) % 6, (dis(gen) % 4) * 90);
      }
   }
}


void GameView::render(const graphics::Context &ctx) const noexcept {
   ctx.set_draw_color(81, 170, 162, 255);
   ctx.clear();

   m_board_view.render(ctx);

   constexpr auto w = 96;
   constexpr auto h = 96;

   const auto &block_pos = m_game.block_pos();
   auto x = static_cast<mb::i32>(block_pos.x);
   auto y = static_cast<mb::i32>(block_pos.y);

   ctx.set_draw_color(214, 203, 108, 255);
   ctx.draw_line(x, y, x + w, y);
   ctx.draw_line(x + w, y, x + w, y + h);
   ctx.draw_line(x + w, y + h, x, y + h);
   ctx.draw_line(x, y + h, x, y);

   ctx.draw(ResourceManager::texture(TextureResource::YellowFigure), x, y, w, h);

   ctx.present();
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
      m_board_view.place_random_tile(m_last_mouse_x, m_last_mouse_y);
      break;
   case SDL_BUTTON_MIDDLE:
      m_middle_button_pressed = true;
      break;
   case SDL_BUTTON_RIGHT:
      m_board_view.rotate_tile(m_last_mouse_x, m_last_mouse_y);
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
   }

   m_game.set_block_pos(x, y);

   m_last_mouse_x = x;
   m_last_mouse_y = y;
}

void GameView::on_mouse_wheel(int y) {
   m_board_view.move_cam(m_last_mouse_x, m_last_mouse_y);
   m_board_view.scale(8*y);
   m_board_view.move_cam(-m_last_mouse_x, -m_last_mouse_y);
}

}// namespace carcassonne::frontend