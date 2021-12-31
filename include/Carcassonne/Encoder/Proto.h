#ifndef CARCASSONNE_ENCODER_PROTO_H
#define CARCASSONNE_ENCODER_PROTO_H

#include <Carcassonne/Proto/Carcassonne.pb.h>
#include <Carcassonne/Direction.h>
#include <Carcassonne/Move.h>

namespace carcassonne::encoder {

[[nodiscard]] constexpr proto::Direction direction_to_proto(Direction dir) {
   return static_cast<proto::Direction>(dir);
}

[[nodiscard]] inline proto::Move move_to_proto(const FullMove &move) {
   proto::Move result{};
   result.set_direction(direction_to_proto(move.direction));
   result.set_x(move.x);
   result.set_y(move.y);
   result.set_rotation(move.rotation);
   result.set_skip_figure(move.ignored_figure);
   return result;
}

}

#endif//CARCASSONNE_ENCODER_PROTO_H
