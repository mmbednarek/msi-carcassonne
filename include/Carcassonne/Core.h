#ifndef MSI_CARCASSONNE_CORE_H
#define MSI_CARCASSONNE_CORE_H
#include <mb/int.h>

namespace carcassonne {

struct Position {
   mb::f64 x, y;
};

class IGame {
 public:
   [[nodiscard]] virtual const Position &block_pos() const noexcept = 0;
   virtual void set_block_pos(mb::f64 x, mb::f64 y) noexcept = 0;
};

}// namespace carcassonne

#endif//MSI_CARCASSONNE_CORE_H
