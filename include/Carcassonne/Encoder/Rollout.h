#ifndef CARCASSONNE_ROLLOUT_H
#define CARCASSONNE_ROLLOUT_H

#include <Carcassonne/Proto/Carcassonne.pb.h>
#include <string_view>
#include <memory>

namespace carcassonne {
struct FullMove;
namespace game {
class Game;
}
}

namespace carcassonne::encoder {

class Rollout {
   proto::Rollout m_rollout{};

 public:
   explicit Rollout(int player_count, std::uint64_t seed);

   void register_move(const FullMove &move);
   void save_rollout(std::string_view filename) const;
   void load_rollout(std::string_view filename);

   void encode(std::ostream &filename) const;
   void decode(std::istream &filename);

   std::unique_ptr<game::Game> as_game();
};

}// namespace carcassonne::encoder

#endif//CARCASSONNE_ROLLOUT_H
