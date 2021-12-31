#ifndef CARCASSONNE_DECODER_PROTO_H
#define CARCASSONNE_DECODER_PROTO_H

#include <Carcassonne/Proto/Carcassonne.pb.h>
#include <Carcassonne/Direction.h>
#include <Carcassonne/Move.h>

namespace carcassonne::decoder {

[[nodiscard]] constexpr Direction direction_from_proto(proto::Direction dir) {
    return static_cast<Direction>(dir);
}

[[nodiscard]] constexpr FullMove move_from_proto(const proto::Move &move) {
   FullMove result{};
   result.direction = direction_from_proto(move.direction());
   result.x = move.x();
   result.y = move.y();
   result.rotation = move.rotation();
   result.ignored_figure = move.skip_figure();
   return result;
}

}

#endif//CARCASSONNE_DECODER_PROTO_H
