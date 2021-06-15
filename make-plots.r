library(ggplot2)
library(emojifont)


mcts_log = read.csv("logs/mcts.csv")


# AI WINS
ggplot(mcts_log, aes(x = move_number, y = ai_wins)) + geom_line(color = "#CC6666", size = 2) +
    labs(x = "Ruch", y = "Zwycięstwa MCTS w wybranym ruchu")
ggsave("plots/ai_wins.pdf")

# UCT
ggplot(mcts_log, aes(x = move_number, y = best_utc)) + geom_line(color = "#CC6666", size = 2) +
    labs(x = "Ruch", y = "UCT1")
ggsave("plots/utc.pdf")

# Selected node sim count
ggplot(mcts_log, aes(x = move_number, y = best_utc)) + geom_line(color = "#CC6666", size = 2) +
    labs(x = "Ruch", y = "Liczba symulacji wybranego wierzchołka")
ggsave("plots/node_simcount.pdf")

# Selected node sim count
ggplot(mcts_log, aes(x = move_number, y = rollout_count)) + geom_line(color = "#CC6666", size = 2) +
    labs(x = "Ruch", y = "Całkowita liczba symulacji")
ggsave("plots/rollout.pdf")

# Node count
ggplot(mcts_log, aes(x = move_number, y = node_count)) + geom_line(color = "#CC6666", size = 2) +
    labs(x = "Ruch", y = "Liczba wierzchołków w drzewie")
ggsave("plots/tree.pdf")

all_heu = read.csv("csv/all_heuristic_params.csv")
all_heu_reduced = all_heu[seq(1, nrow(all_heu), 300), ]

# All heuristic
ggplot(all_heu_reduced, aes(x = iteration, y = moastery_score)) + geom_point(color = "#6666CC", size = 2) +
    labs(x = "Iteracja", y = "monastery_score")
ggsave("plots/all_heuristic/monastery_score.pdf")

ggplot(all_heu_reduced, aes(x = iteration, y = min_figure_count)) + geom_point(color = "#6666CC", size = 2) +
    labs(x = "Iteracja", y = "min_figure_count")
ggsave("plots/all_heuristic/figure_count.pdf")

ggplot(all_heu_reduced, aes(x = iteration, y = grass_score)) + geom_point(color = "#6666CC", size = 2) +
    labs(x = "Iteracja", y = "grass_score")
ggsave("plots/all_heuristic/grass_score.pdf")

ggplot(all_heu_reduced, aes(x = iteration, y = tile_type_score)) + geom_point(color = "#6666CC", size = 2) +
    labs(x = "Iteracja", y = "tile_type_score")
ggsave("plots/all_heuristic/tile_type_score.pdf")

ggplot(all_heu_reduced, aes(x = iteration, y = tile_close_score)) + geom_point(color = "#6666CC", size = 2) +
    labs(x = "Iteracja", y = "tile_close_score")
ggsave("plots/all_heuristic/tile_close_score.pdf")

gen = read.csv("csv/gen_heuristic_params.csv")
gen_reduced = gen[seq(1, nrow(all_heu), 20), ]

ggplot(gen_reduced, aes(x = generation, y = moastery_score)) + geom_point(color = "#6666CC", size = 2) +
    labs(x = "Generacja", y = "monastery_score")
ggsave("plots/generations/monastery_score.pdf")

ggplot(gen_reduced, aes(x = generation, y = min_figure_count)) + geom_point(color = "#6666CC", size = 2) +
    labs(x = "Generacja", y = "min_figure_count")
ggsave("plots/generations/figure_count.pdf")

ggplot(gen_reduced, aes(x = generation, y = grass_score)) + geom_point(color = "#6666CC", size = 2) +
    labs(x = "Generacja", y = "grass_score")
ggsave("plots/generations/grass_score.pdf")

ggplot(gen_reduced, aes(x = generation, y = tile_type_score)) + geom_point(color = "#6666CC", size = 2) +
    labs(x = "Generacja", y = "tile_type_score")
ggsave("plots/generations/tile_type_score.pdf")

ggplot(gen_reduced, aes(x = generation, y = tile_close_score)) + geom_point(color = "#6666CC", size = 2) +
    labs(x = "Generacja", y = "tile_close_score")
ggsave("plots/generations/tile_close_score.pdf")

switch = read.csv("csv/heuristic_params_per_switch.csv")

ggplot(switch, aes(x = switching_players_count, y = moastery_score)) + geom_point(color = "#6666CC", size = 2) +
    labs(x = "Liczba zamian graczy", y = "monastery_score")
ggsave("plots/switch/monastery_score.pdf")

ggplot(switch, aes(x = switching_players_count, y = min_figure_count)) + geom_point(color = "#6666CC", size = 2) +
    labs(x = "Liczba zamian graczy", y = "min_figure_count")
ggsave("plots/switch/figure_count.pdf")

ggplot(switch, aes(x = switching_players_count, y = grass_score)) + geom_point(color = "#6666CC", size = 2) +
    labs(x = "Liczba zamian graczy", y = "grass_score")
ggsave("plots/switch/grass_score.pdf")

ggplot(switch, aes(x = switching_players_count, y = tile_type_score)) + geom_point(color = "#6666CC", size = 2) +
    labs(x = "Liczba zamian graczy", y = "tile_type_score")
ggsave("plots/switch/tile_type_score.pdf")

ggplot(switch, aes(x = switching_players_count, y = tile_close_score)) + geom_point(color = "#6666CC", size = 2) +
    labs(x = "Liczba zamian graczy", y = "tile_close_score")
ggsave("plots/switch/tile_close_score.pdf")
