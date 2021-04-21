#ifndef MSI_CARCASSONNE_GAME_H
#define MSI_CARCASSONNE_GAME_H
#include "EventManager.h"
#include "Render.h"
#include <mb/int.h>

namespace msi::game {

enum class Status {
   Init,
   Running,
   Quitting,
};

struct Pos {
   mb::f64 x, y;
};

class Game {
   Status m_status = Status::Init;
   EventManager m_event_manager;
   Render m_render;
   Pos m_block_pos{20.0f, 20.0f};

 public:
   Game();

   void update();

   constexpr void set_status(Status s) {
      m_status = s;
   }

   [[nodiscard]] constexpr Status status() const {
      return m_status;
   }

   [[nodiscard]] constexpr EventManager &event_manager() {
      return m_event_manager;
   }

   [[nodiscard]] constexpr Render &render() {
      return m_render;
   }

    [[nodiscard]] constexpr Pos &block_pos() {
        return m_block_pos;
    }
};

}// namespace msi::game

#endif//MSI_CARCASSONNE_GAME_H
