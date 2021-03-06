#include <Carcassonne/Frontend/GameView.h>
#include <Carcassonne/Frontend/Resource.h>
#include <Carcassonne/Game/Game.h>
#include <mb/int.h>
#include <random>

namespace carcassonne::frontend {

GameView::GameView(IGame &game, PlayerAssignment controlled_players) : m_game(game),
                                                                       m_move(),
                                                                       m_camera(game.board()),
                                                                       m_board_view(game.board(), m_camera),
                                                                       m_figures_view(game, m_camera),
                                                                       m_place_tile_view(game.board(), m_camera, m_move),
                                                                       m_place_figure_view(m_camera, m_move),
                                                                       m_score_board_view(game.scores()), m_controlled_players(controlled_players) {
   game.on_next_move([this](IGame &game, Player p, FullMove) {
      if (m_controlled_players & p) {
         next_move();
      }
   });
}

void GameView::render(const graphics::Context &ctx) const noexcept {
   ctx.set_draw_color(39, 79, 136, 255);
   ctx.clear();

   m_board_view.render(ctx);

   if (m_move) {
      switch (m_move->phase()) {
      case MovePhase::PlaceTile:
         m_place_tile_view.render(ctx);
         break;
      case MovePhase::PlaceFigure:
         m_place_figure_view.render(ctx);
         break;
      case MovePhase::Done: break;
      }
   }

   m_figures_view.render(ctx);
   m_score_board_view.render(ctx);

   ctx.present();
}

void GameView::update(double dt) {
   m_board_view.update(dt);
   if (m_move) {
      m_place_tile_view.update(dt);
   }
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
      if (m_move) {
         switch (m_move->phase()) {
         case MovePhase::PlaceTile:
            m_place_tile_view.place_tile();
            break;
         case MovePhase::PlaceFigure:
            m_place_figure_view.place_figure();
            break;
         case MovePhase::Done: break;
         }
      }
      break;
   case SDL_BUTTON_MIDDLE:
      m_middle_button_pressed = true;
      break;
   case SDL_BUTTON_RIGHT:
      if (m_move) {
         switch (m_move->phase()) {
         case MovePhase::PlaceTile:
            m_place_tile_view.rotate_tile();
            break;
         case MovePhase::PlaceFigure:
            m_place_figure_view.ignore_figure();
            break;
         case MovePhase::Done: break;
         }
      }
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
      m_camera.move(m_last_mouse_x - x, m_last_mouse_y - y);
   } else if (m_move) {
      switch (m_move->phase()) {
      case MovePhase::PlaceTile:
         m_place_tile_view.select_tile(x, y);
         break;
      case MovePhase::PlaceFigure:
         m_place_figure_view.update_direction(x, y);
         break;
      case MovePhase::Done: break;
      }
   }

   m_last_mouse_x = x;
   m_last_mouse_y = y;
}

void GameView::on_mouse_wheel(int y) {
   m_camera.move(m_last_mouse_x, m_last_mouse_y);
   m_camera.scale_up(8 * y);
   m_camera.move(-m_last_mouse_x, -m_last_mouse_y);
}

void GameView::next_move() {
   m_move = m_game.new_move(m_game.current_player());
}

}// namespace carcassonne::frontend