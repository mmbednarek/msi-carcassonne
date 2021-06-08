#include <Carcassonne/Game/Game.h>
#include <Evolution/DiffEvol.h>
#include <Carcassonne/Parameters.h>
#include <Carcassonne/AI/HeuristicPlayer.h>
// #include <boost/program_options.hpp>
#include <fmt/core.h>

constexpr auto g_population_size = 36;
constexpr auto g_generations_count = 200;
constexpr auto g_mutation_chance = 0.9;
constexpr auto g_cross_chance = .8;
constexpr auto g_mutation_rate_initial = 0.2;
constexpr auto g_mutation_rate_final = 0.05;
constexpr auto g_optimal_fitness = 300;

std::array<carcassonne::ai::HeuristicPlayer, 4> g_heuristic_players{
        carcassonne::ai::HeuristicPlayer(carcassonne::Player::Black),
        carcassonne::ai::HeuristicPlayer(carcassonne::Player::Blue),
        carcassonne::ai::HeuristicPlayer(carcassonne::Player::Red),
        carcassonne::ai::HeuristicPlayer(carcassonne::Player::Yellow),
};

evolution::ObjectiveFunction make_objective_function(util::IRandomGenerator &rand, carcassonne::Parameters &params, evolution::Parameters &evo_params) {
   return [&evo_params,
           &rand](const evolution::Variables &vars) -> double {
      carcassonne::Parameters params;
      params.moastery_score = vars.moastery_score;
      params.grass_penalty = vars.grass_penalty;
      params.min_figure_count = vars.min_figure_count;
      params.grass_score = vars.grass_score;
      params.tile_type_score = vars.tile_type_score;
      params.tile_close_score = vars.tile_close_score;
      params.tile_open_score = vars.tile_open_score;
      
      // carcassonne::game::Game game(2, 0);

      // std::random_device random_device;
      // std::mt19937 random_generator(random_device());

      // std::vector<carcassonne::ai::HeuristicPlayer> heuristic_players;
      // heuristic_players.reserve(2);
      // std::transform(carcassonne::g_players.begin(), carcassonne::g_players.begin() + 2, std::back_inserter(heuristic_players), [](carcassonne::Player p) {
      //    return carcassonne::ai::HeuristicPlayer(p);
      // });
      // for (auto &player : heuristic_players) {
      //    player.await_turn(game);
      // }
      // auto simulated_game = game;
      // for (auto move_index = simulated_game.move_index(); move_index < carcassonne::g_max_moves; ++move_index) {
      //    auto current_player = simulated_game.current_player();
      //    auto full_move = g_heuristic_players[static_cast<mb::size>(current_player)].make_move(simulated_game).unwrap();
      //    simulated_game.update(0);
      // }
      // find_winner();

      fmt::print("\nfinal result:");
      fmt::print("\n  moastery_score:\t\t{}, ", params.moastery_score);
      fmt::print("\n  grass_penalty:\t\t{}, ", params.grass_penalty);
      fmt::print("\n  min_figure_count:\t\t{}, ", params.min_figure_count);
      fmt::print("\n  grass_score:\t\t{}, ", params.grass_score);
      fmt::print("\n  tile_type_score:\t\t{}, ", params.tile_type_score);
      fmt::print("\n  tile_close_score:\t\t{}, ", params.tile_close_score);
      fmt::print("\n  tile_open_score:\t\t{}, ", params.tile_open_score);
      // fmt::print("\n  Fitness: {}\n", result);
      return 0;//result;
   };
}


constexpr bool player_range_ok(int count) {
   return count >= 0 && count <= 4;
}


int main(int argc, char **argv) {

   util::Random rand;
   carcassonne::Parameters params;

   evolution::Constraint constraint{
           {1, 5000},  // moastery_score
           {1, 100},    // grass_penalty
           {2, 7},      // min_figure_count
           {1, 100},    // grass_score
           {1, 100},    // tile_type_score
           {1, 50000},  // tile_close_score
           {1, 1000},   // tile_open_score
   };

   evolution::Parameters evo_params{
      .population_size = g_population_size,
      .generations_count = g_generations_count,
      .mutation_chance = g_mutation_chance,
      .cross_chance = g_cross_chance,
      .mutation_rate_initial = g_mutation_rate_initial,
      .mutation_rate_final = g_mutation_rate_final,
      .optimal_fitness = g_optimal_fitness,
   };

   auto objective_func = make_objective_function(rand, params, evo_params);
   auto result = evolution::FindOptimal(rand, objective_func, evo_params, constraint);

}
