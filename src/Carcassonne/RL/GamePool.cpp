#include <Carcassonne/RL/GamePool.h>
#include <Carcassonne/Game/Game.h>

namespace carcassonne::ai::rl {

GamePool g_game_pool_instance;

GamePool::GamePool() : m_pool(32) {}

game::Game *GamePool::new_game(const game::Game &game) {
   return m_pool.construct(game);
}

void GamePool::free(game::Game *game) {
   m_pool.free(game);
}

GamePool &GamePool::the() {
   return g_game_pool_instance;
}

PoolGame::PoolGame(IGame &game) : m_game(GamePool::the().new_game(*dynamic_cast<game::Game *>(&game))) {
   m_game->clear_callbacks();
}

PoolGame::~PoolGame() {
   if (m_game != nullptr) {
      GamePool::the().free(m_game);
   }
}

}