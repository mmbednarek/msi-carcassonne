#ifndef MSI_CARCASSONNE_AI_H
#define MSI_CARCASSONNE_AI_H
#include <mb/int.h>
#include <memory>

namespace carcassonne::ai {

class Node;
using NodePtr = Node *;
using NodeUPtr = const std::unique_ptr<Node> &;

}// namespace carcassonne::ai

#endif//MSI_CARCASSONNE_AI_H
