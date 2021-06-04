#include <Carcassonne/AI/Tree.h>
#include <cassert>
#include <chrono>
#include <cmath>
#include <fmt/core.h>

namespace carcassonne::ai {

constexpr NodeId g_root_node = 0;

Tree::Tree(const IGame &game, const Player &player)
    : m_player(player), m_player_count(game.player_count()) {
   m_nodes.emplace_back(game.clone(), player, FullMove{});
   expansion(g_root_node);
}

FullMove Tree::find_non_idiotic() {
   auto &children = m_nodes[g_root_node].children();
   auto move_it = std::find_if(children.begin(), children.end(), [this](NodeId child_id) {
      return m_nodes[child_id].m_player_wins[static_cast<int>(m_player)] > 0;
   });
   return m_nodes[*move_it].move();
}

FullMove Tree::find_best_move() {
   selection(g_root_node, 0);
   auto until = std::chrono::steady_clock::now() + std::chrono::milliseconds{5000};
   while (std::chrono::steady_clock::now() < until) {
      selection(g_root_node, 0);
   }
   auto &node = m_nodes[select_best_node()];
   fmt::print("visitation count: {}\n", m_nodes[g_root_node].m_visitation_count);
   fmt::print("win ratio: {}\n", node.win_ratio());
   return node.move();
}

void Tree::selection(NodeId node_id, int level) {
   auto &current_node = m_nodes[node_id];
   const auto rollout_count = m_nodes[g_root_node].m_visitation_count;
   const auto &children = current_node.children();

   auto selected_child_it = std::max_element(
           children.begin(),
           children.end(),
           [this, rollout_count](NodeId lhs, NodeId rhs) -> bool {
              const auto &lhs_node = m_nodes[lhs];
              const auto &rhs_node = m_nodes[rhs];
              return lhs_node.UCT1(rollout_count) < rhs_node.UCT1(rollout_count);
           });

   assert(selected_child_it != children.end());
   const auto child_id = *selected_child_it;
   auto &child_node = m_nodes[child_id];

   if (!child_node.expanded()) {
      expansion(child_id);
      return;
   }

   if (!m_nodes[child_id].children().empty()) {
      selection(child_id, level + 1);
   }
}

void Tree::expansion(NodeId node_id) noexcept {
   auto &game = m_nodes[node_id].game();
   const auto current_player = game.current_player();
   for (auto tile_location : game.moves()) {
      auto game_clone = game.clone();
      auto move = game_clone->new_move(current_player);
      move->place_tile(tile_location);

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

         m_nodes.emplace_back(new_id, std::move(game_clone_clone), current_player, full_move, node_id);
         auto &new_node = m_nodes[new_id];
         new_node.simulation();
         backpropagation(new_id);

         m_nodes[node_id].add_child(new_id);
      }

      move->ignore_figure();
      game_clone->update(0);
      {
         const auto new_id = m_nodes.size();
         const auto full_move = FullMove{
                 .x = tile_location.x,
                 .y = tile_location.y,
                 .rotation = tile_location.rotation,
                 .ignored_figure = true,
         };

         m_nodes.emplace_back(new_id, std::move(game_clone), current_player, full_move, node_id);
         auto &new_node = m_nodes[new_id];
         new_node.simulation();
         backpropagation(new_id);

         m_nodes[node_id].add_child(new_id);
      }
   }

   m_nodes[node_id].mark_as_expanded();
}

void Tree::backpropagation(NodeId node_id) {
   while (node_id != 0) {
      auto &node = m_nodes[node_id];
      auto &parent = m_nodes[node.parent_id()];

      for (auto i = 0; i < m_player_count; ++i) {
         parent.m_player_wins[i] += node.m_player_wins[i];
      }
      parent.m_visitation_count += node.m_visitation_count;
      node_id = parent.id();
   }
}

NodeId Tree::select_best_node() {
   auto &root_node = m_nodes[g_root_node];
   const auto &children = root_node.children();

   auto selected_child_it = std::max_element(
           children.begin(),
           children.end(),
           [this](NodeId lhs, NodeId rhs) -> bool {
              const auto &lhs_node = m_nodes[lhs];
              const auto &rhs_node = m_nodes[rhs];
              return lhs_node.win_ratio() < rhs_node.win_ratio();
           });
   assert(selected_child_it != children.end());
   return *selected_child_it;
}

void Tree::change_root(NodeId id) {
   std::for_each(m_nodes.begin(), m_nodes.end(), [](Node &node) {
      node.clear_children();
   });

   std::vector<bool> to_erase(m_nodes.size());
   std::transform(m_nodes.begin(), m_nodes.end(), to_erase.begin(), [this, new_id = 0, expected = id](const Node &node) mutable {
      auto child = node.id();
      while (child != 0) {
         if (child == expected) {
            auto &child_node = m_nodes[expected];
            auto &parent_node = m_nodes[child_node.parent_id()];
            child_node.update_id(new_id);
            child_node.update_parent_id(parent_node.id());
            parent_node.add_child(new_id);
            ++new_id;
            return false;
         }
         child = m_nodes[expected].id();
      }
      return true;
   });

   std::erase_if(m_nodes, [&to_erase, index = 0](const Node &node) mutable {
      return to_erase[index++];
   });
}


}// namespace carcassonne::ai