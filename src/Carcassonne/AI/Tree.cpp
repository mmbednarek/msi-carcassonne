#include <Carcassonne/AI/Tree.h>
#include <chrono>
#include <cmath>
#include <limits>

namespace carcassonne::ai {

Tree::Tree(IGame &game, const Player &player)
 : m_player(player)
 , m_root(Node(game, player, m_rollouts_performed_count))
{
   m_root.id() = 0;
   m_nodes.push_back(m_root);
   expansion(m_root, m_rollouts_performed_count);
   find_best_move(game, m_rollouts_performed_count);
}

void Tree::find_best_move(const IGame &game, mb::u64 &rollouts_performed_count) {
   auto root_ref = std::ref(m_root);
   auto node_ref = selection(root_ref, rollouts_performed_count);
   using namespace std::literals;
   const std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
   const std::chrono::time_point<std::chrono::system_clock> end;
   for (
      auto start = std::chrono::steady_clock::now(), now = start; 
      now < start + std::chrono::milliseconds{3600000};
      now = std::chrono::steady_clock::now() )
   {
      backpropagation(node_ref);
      node_ref = selection(m_root, rollouts_performed_count);
      m_rollouts_performed_count++;
   }
   backpropagation(node_ref);
}

std::tuple<TileMove, Direction> Tree::best_move(IGame &game) noexcept {
   TileMove tile_placement;
   Direction figure_placement;
   // BEST_MOVE
   return std::make_tuple(tile_placement, figure_placement);
}

RefNode Tree::selection(RefNode current_node, mb::u64 &rollouts_performed_count) {
   double best_uct = 0;
   auto selected_node = current_node;
   auto selected_child_it = std::max_element(
      current_node.get().children().begin(), 
      current_node.get().children().end(), 
      [&rollouts_performed_count](const RefNode &n_uptr1, const RefNode &n_uptr2) -> bool {
      return n_uptr1.get().UCT1(rollouts_performed_count) < n_uptr2.get().UCT1(rollouts_performed_count);
   });
   if (selected_child_it == current_node.get().children().end())
      selected_node = *selected_child_it;
   if((*selected_child_it).get().children().size() == 0) { // is it a leaf node?
      if((*selected_child_it).get().m_visitation_count == 0) { // has the node been visited?
         (*selected_child_it).get().simulation();
         rollouts_performed_count++;
         selected_node = *selected_child_it;
      } else {
         expansion(*selected_child_it, rollouts_performed_count);
         selected_node = selection(*selected_child_it, rollouts_performed_count);
      }
   } else {
      selected_node = selection(*selected_child_it, rollouts_performed_count);
   }
   return selected_node;
}

void Tree::expansion(RefNode selected_node, mb::u64 &rollouts_performed_count) noexcept {
   // auto previous_player = selected_node.get().parent().get().player();
   // auto players_count = selected_node.get().parent().get().game().player_count();
   // auto current_player = next_player(previous_player, players_count);
   auto current_player = selected_node.get().game().current_player();
   auto players_count = selected_node.get().game().player_count();
   auto move = selected_node.get().game().new_move(m_player);
   const auto possible_tile_moves = selected_node.get().game().moves(move->tile_type());
   const auto possible_tile_moves_debug = std::vector<TileMove>(possible_tile_moves.begin(), possible_tile_moves.end());
   for (TileMove possible_tile_move : possible_tile_moves) {
      auto game_clone = selected_node.get().game().clone();
      auto move = game_clone->new_move(m_player);
      move->place_tile(possible_tile_move.x, possible_tile_move.y, possible_tile_move.rotation);
      const auto possible_figure_moves = game_clone->figure_placements(possible_tile_move.x, possible_tile_move.y);
      for (Direction possible_figure_move : possible_figure_moves) {
         auto game_clone_clone = game_clone->clone();
         auto move_clone = game_clone_clone->new_move(m_player);
         move_clone->place_figure(possible_figure_move);
         auto NextPlayer = next_player(current_player, players_count);
         auto parent = selected_node;
         auto new_id = (m_nodes.end() - 1)->id() + 1;
         m_nodes.emplace_back(Node(*game_clone_clone, static_cast<Player>(NextPlayer), rollouts_performed_count, parent));
         (m_nodes.end() - 1)->id() = new_id;
         auto new_node_ref = std::ref(*(m_nodes.end() - 1));
         selected_node.get().children().push_back(new_node_ref);
      }
   }
}

void Tree::backpropagation(RefNode node) {
   // BACKPROPAGATION
   auto n_ref = node;
   auto p_ref = node.get().parent();
   auto n = node.get();
   auto p = node.get().parent().get();
   if (node.get().parent().get().id() != node.get().id()) {
      node.get().parent().get().m_wins_count += node.get().m_wins_count;
      node.get().parent().get().m_loses_count += node.get().m_loses_count;
      node.get().parent().get().m_visitation_count += node.get().m_visitation_count;
      backpropagation(node.get().parent());
   } else {
      return;
   }
}



}// namespace carcassonne::ai