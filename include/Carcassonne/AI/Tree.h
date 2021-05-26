#ifndef MSI_CARCASSONNE_TREE_H
#define MSI_CARCASSONNE_TREE_H
#include <Carcassonne/IGame.h>
#include <mb/int.h>

namespace carcassonne::ai {

class Tree {
   public:
   Tree();
   Node selection();
   bool simulation();
   Node expansion();
   void backpropagation();
   void updateUCT();
};

} // namespace carcassonne::game::ai

#endif