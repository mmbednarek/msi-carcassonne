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
   auto node_id = selection(g_root_node, rollouts_performed_count);
   auto until = std::chrono::steady_clock::now() + std::chrono::milliseconds{2000};
   while (std::chrono::steady_clock::now() < until) {
      backpropagation(m_nodes[g_root_node]);
      node_id = selection(g_root_node, rollouts_performed_count);
      m_rollouts_performed_count++;
   }
   backpropagation(m_nodes[node_id]);
   auto &node = m_nodes[node_id];
   return node.move();
}

NodeId Tree::selection(NodeId node_id, mb::u64 &rollouts_performed_count) {
   auto &current_node = m_nodes[node_id];
   const auto &children = current_node.children();
   auto selected_child_it = std::max_element(
           children.begin(),
           children.end(),
           [this, &rollouts_performed_count](const NodeId &lhs, const NodeId &rhs) -> bool {
              return m_nodes[lhs].UCT1(rollouts_performed_count) < m_nodes[rhs].UCT1(rollouts_performed_count);
           });
   assert(selected_child_it != children.end());
   auto &child_node = m_nodes[*selected_child_it];
   auto child_id = child_node.id();

   if (!child_node.children().empty()) {
      child_node.simulation();
      rollouts_performed_count++;
      return child_id;
   }

   if (child_node.m_visitation_count == 0) {
      expansion(child_id, rollouts_performed_count);
   }

   if (m_nodes[child_id].children().empty()) {
      return child_id;
   }
   return selection(child_id, rollouts_performed_count);
}

void Tree::expansion(NodeId node_id, mb::u64 &rollouts_performed_count) noexcept {
   auto &game = m_nodes[node_id].game();
   const auto current_player = game.current_player();
   const auto players_count = game.player_count();
   for (auto tile_location : game.moves()) {
      auto game_clone = game.clone();
      auto move = game_clone->new_move(m_player);
      move->place_tile(tile_location);
      for (Direction figure_move : game_clone->figure_placements(tile_location.x, tile_location.y)) {
         auto game_clone_clone = game_clone->clone();

         {
            auto move_clone = move->clone(*game_clone_clone);
            move_clone->place_figure(figure_move);
         }

         const auto new_id = m_nodes.size();

         const auto full_move = FullMove{
                 .x = tile_location.x,
                 .y = tile_location.y,
                 .rotation = tile_location.rotation,
                 .ignored_figure = false,
                 .direction = figure_move,
         };

         m_nodes.emplace_back(new_id, std::move(game_clone_clone), next_player(current_player, players_count), full_move, node_id);
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


}// namespace carcassonne::ai