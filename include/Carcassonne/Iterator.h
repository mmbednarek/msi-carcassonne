#ifndef CARCASSONNE_ITERATOR_H
#define CARCASSONNE_ITERATOR_H
#include "IGame.h"

namespace carcassonne {

struct EveryMoveIter {
   FullMove m_move;
 public:
   using self_type = EveryMoveIter;
   using value_type = FullMove;
   using reference = FullMove &;
   using const_reference = const FullMove &;
   using pointer = FullMove *;
   using const_pointer = const FullMove *;
   using iterator_category = std::bidirectional_iterator_tag;
   using difference_type = mb::size;

   constexpr EveryMoveIter() : m_move{
                                       .x = 0,
                                       .y = 0,
                                       .rotation = 0,
                                       .ignored_figure = false,
                                       .direction = Direction::North,
                               } {}
};

}
#endif//CARCASSONNE_ITERATOR_H
