#ifndef MSI_CARCASSONNE_NODE_H
#define MSI_CARCASSONNE_NODE_H
#include <Carcassonne/IGame.h>
#include <mb/int.h>
// #include <map>

// Node represents the state resulted by choosing an action

namespace carcassonne::ai {

class Node {
  mb::u32 visited_time_;
  float average_profit_;
  bool is_in_search_;
//   std::map<PositionIndex, HashKey> child_hash_keys_;

 public:
  Node(int32_t visited_time, float average_profit, bool is_in_search)
      : visited_time_(visited_time),
        average_profit_(average_profit),
        is_in_search_(is_in_search) {}
};

} // namespace carcassonne::game::ai

#endif //MSI_CARCASSONNE_NODE_H
