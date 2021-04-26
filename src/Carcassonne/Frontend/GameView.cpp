#include <Carcassonne/Frontend/GameView.h>
#include <Carcassonne/Frontend/Resource.h>
#include <Carcassonne/Game/Game.h>
#include <mb/int.h>
#include <random>

namespace carcassonne::frontend {

std::array<std::pair<int, int>, 100> g_board{};

GameView::GameView(IGame &game) : m_game(game) {
   std::random_device rd;
   std::mt19937_64 gen(rd());
   std::uniform_int_distribution<int> dis;

   for (int y = 0; y < 10; ++y) {
      for (int x = 0; x < 10; ++x) {
         g_board[x + y * 10] = std::make_pair(dis(gen) % 4, dis(gen) % 6);
      }
   }
}


void GameView::render(const graphics::Context &ctx) const noexcept {
   ctx.set_draw_color(81, 170, 162, 255);
   ctx.clear();

   for (int y = 0; y < 10; ++y) {
      for (int x = 0; x < 10; ++x) {
         auto tile = g_board[x + y * 10];
         ctx.draw(ResourceManager::texture(TextureResource::Tiles), 96 * tile.first, 96 * tile.second, 96, 96, 96 * x, 96 * y, 96, 96, 0);
      }
   }

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
}

void GameView::on_button_up(std::uint8_t button) {
}

void GameView::on_mousemove(int x, int y) {
   m_game.set_block_pos(x, y);
}

}// namespace carcassonne::frontend