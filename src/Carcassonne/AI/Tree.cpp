#include <Carcassonne/AI/Tree.h>
#include <cassert>
#include <chrono>
#include <cmath>

namespace carcassonne::ai {

constexpr NodeId g_root_node = 0;

Tree::Tree(const IGame &game, const Player &player)
    : m_player(player) {
   m_nodes.emplace_back(game.clone(), player, FullMove{});
   expansion(g_root_node, m_rollouts_performed_count);
}

FullMove Tree::find_best_move(const IGame &game, mb::u64 &rollouts_performed_count) {
   selection(g_root_node, 0, rollouts_performed_count);
   //   auto until = std::chrono::steady_clock::now() + std::chrono::milliseconds{6000000};
   auto until = std::chrono::steady_clock::now() + std::chrono::milliseconds{5000};
   while (std::chrono::steady_clock::now() < until) {
      selection(g_root_node, 0, rollouts_performed_count);
      m_rollouts_performed_count++;
   }
   auto &node = m_nodes[select_best_node(m_rollouts_performed_count)];
   return node.move();
}

NodeId Tree::selection(NodeId node_id, int level, mb::u64 &rollouts_performed_count) {
   if (level > 2)
      return node_id;

   auto &current_node = m_nodes[node_id];
   const auto &children = current_node.children();

   // not expanded
   auto selected_child_it = std::max_element(
           children.begin(),
           children.end(),
           [this, &rollouts_performed_count](NodeId lhs, NodeId rhs) -> bool {
              const auto &lhs_node = m_nodes[lhs];
              const auto &rhs_node = m_nodes[rhs];
              const auto lhs_uct = lhs_node.UCT1(rollouts_performed_count);
              const auto rhs_uct = rhs_node.UCT1(rollouts_performed_count);
              if (lhs_uct == rhs_uct) {
                 if (!lhs_node.expanded())
                    return false;
                 return true;
              }
              return lhs_uct < rhs_uct;
           });

   assert(selected_child_it != children.end());
   auto child_id = *selected_child_it;
   auto &child_node = m_nodes[child_id];

   if (!child_node.expanded()) {
      expansion(child_id, rollouts_performed_count);
      return child_id;
   }
   if (m_nodes[child_id].children().empty()) {
      return child_id;
   }
   return selection(child_id, level + 1, rollouts_performed_count);
}

void Tree::expansion(NodeId node_id, mb::u64 &rollouts_performed_count) noexcept {
   m_nodes[node_id].mark_as_expanded();
   auto &game = m_nodes[node_id].game();
   const auto current_player = game.current_player();
   const auto players_count = game.player_count();
   for (auto tile_location : game.moves()) {
      auto game_clone = game.clone();
      auto move = game_clone->new_move(m_player);
      move->place_tile(tile_location);
      auto score = game.move_score(m_player, move->tile_type(), tile_location);
      if (score.second < 10 && !m_nodes[node_id].children().empty()) {
         continue;
      }

      for (Direction figure_move : game_clone->figure_placements(tile_location.x, tile_location.y)) {
         auto game_clone_clone = game_clone->clone();

         {
            auto move_clone = move->clone(*game_clone_clone);
            move_clone->place_figure(figure_move);
         }
         game_clone_clone->update(0);

         const auto new_id = m_nodes.size();

         const auto full_move = FullMove{
                 .x = tile_location.x,
                 .y = tile_location.y,
                 .rotation = tile_location.rotation,
                 .ignored_figure = false,
                 .direction = figure_move,
         };

         m_nodes.emplace_back(new_id, std::move(game_clone_clone), next_player(current_player, players_count), full_move, node_id);
         auto &new_node = m_nodes[new_id];
         new_node.simulation();
         backpropagation(new_node);

         m_nodes[node_id].add_child(new_id);
      }
   }
}

void Tree::backpropagation(Node &node) {
   if (node.parent_id() == 0 && node.id() == 0)
      return;

   auto &parent = m_nodes[node.parent_id()];
   parent.m_wins_count += node.m_wins_count;
   parent.m_loses_count += node.m_loses_count;
   parent.m_visitation_count += node.m_visitation_count;
   backpropagation(parent);
}

NodeId Tree::select_best_node(mb::u64 &rollouts_performed_count) {
   auto &root_node = m_nodes[g_root_node];
   const auto &children = root_node.children();

   auto selected_child_it = std::max_element(
           children.begin(),
           children.end(),
           [this, &rollouts_performed_count](NodeId lhs, NodeId rhs) -> bool {
              const auto &lhs_node = m_nodes[lhs];
              const auto &rhs_node = m_nodes[rhs];
              return lhs_node.UCT1(rollouts_performed_count) < rhs_node.UCT1(rollouts_performed_count);
           });
   assert(selected_child_it != children.end());
   return *selected_child_it;
}


}// namespace carcassonne::ai