#include <Carcassonne/Encoder/GameState.h>

using eres = mb::result<mb::empty>;

namespace carcassonne::encoder {

eres encode_game_state(const IGame &game, IBoolWriter &writer) {
   return mb::ok;
}

}