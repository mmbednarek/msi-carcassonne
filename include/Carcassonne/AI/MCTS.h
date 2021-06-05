#ifndef MSI_CARCASSONNE_MCTS_H
#define MSI_CARCASSONNE_MCTS_H
#include "AI.h"
#include <Carcassonne/Player.h>
#include <mb/int.h>

namespace carcassonne {
struct FullMove;
}

namespace carcassonne::ai {

class Tree;

mb::result<FullMove> find_non_idiotic(Tree &tree, Player player);
void simulate(Tree &tree, NodeId node_id);
void expand(Tree &tree, NodeId node_id);
void backpropagate(Tree &tree, NodeId node_id, mb::size player_count);
void run_selection(Tree &tree);
void run_mcts(Tree &tree, mb::i64 time_limit);
FullMove choose_move(Tree &tree);

}// namespace carcassonne::ai

#endif//MSI_CARCASSONNE_MCTS_H
