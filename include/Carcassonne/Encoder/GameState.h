#ifndef MSI_CARCASSONNE_GAMESTATE_H
#define MSI_CARCASSONNE_GAMESTATE_H
#include <Carcassonne/IGame.h>
#include <mb/result.h>
#include "IBoolWriter.h"

namespace carcassonne::encoder {

mb::result<mb::empty> encode_game_state(const IGame &game, IBoolWriter &writer);

}

#endif//MSI_CARCASSONNE_GAMESTATE_H
