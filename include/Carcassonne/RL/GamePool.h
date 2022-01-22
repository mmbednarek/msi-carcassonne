#ifndef CARCASSONNE_GAMEPOOL_H
#define CARCASSONNE_GAMEPOOL_H
#include <boost/pool/object_pool.hpp>

namespace carcassonne {

class IGame;

namespace game {
class Game;
}

}

namespace carcassonne::ai::rl {

class GamePool {
   boost::object_pool<game::Game> m_pool;

 public:
   GamePool();

   game::Game *new_game(const game::Game &);
   void free(game::Game *game);

   static GamePool &the();
};

class PoolGame {
   game::Game *m_game = nullptr;
 public:
   explicit PoolGame(IGame &game);

   inline PoolGame(const PoolGame &game) : m_game(GamePool::the().new_game(*game.m_game)) {}

   inline PoolGame &operator=(const PoolGame &game) {
      if (m_game != nullptr) {
         GamePool::the().free(m_game);
      }
      m_game = GamePool::the().new_game(*game.m_game);
      return *this;
   }

   inline PoolGame &operator=(game::Game *game) {
      if (m_game != nullptr) {
         GamePool::the().free(m_game);
      }
      m_game = game;
      return *this;
   }

   inline PoolGame(PoolGame &&game) noexcept : m_game(std::exchange(game.m_game, nullptr)) {}

   inline PoolGame &operator=(PoolGame &&game) noexcept {
      m_game = std::exchange(game.m_game, nullptr);
      return *this;
   }

   ~PoolGame();

   constexpr game::Game *operator->() {
      return m_game;
   }

   constexpr game::Game &operator*() const {
      return *m_game;
   }
};

}

#endif//CARCASSONNE_GAMEPOOL_H
