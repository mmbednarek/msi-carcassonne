#ifndef MSI_CARCASSONNE_MCTS_H
#define MSI_CARCASSONNE_MCTS_H
#include "AI.h"
#include <Carcassonne/Player.h>
#include <mb/int.h>

namespace carcassonne {
struct FullMove;
}

namespace carcassonne::ai {

enum class SimulationType {
   Heuristic,
   Random
};

class Tree;

mb::result<FullMove> find_non_idiotic(Tree &tree, Player player);
void simulate(Tree &tree, NodePtr node_id);
void expand(Tree &tree, NodePtr node_id, carcassonne::ai::SimulationType simulation_type);
void backpropagate(Tree &tree, NodePtr node_id, Player winner);
void run_selection(Tree &tree, carcassonne::ai::SimulationType sim_type);
void run_mcts(Tree &tree, mb::i64 time_limit, carcassonne::ai::SimulationType sim_type);
FullMove choose_move(Tree &tree, Player player);

}// namespace carcassonne::ai

#endif//MSI_CARCASSONNE_MCTS_H
