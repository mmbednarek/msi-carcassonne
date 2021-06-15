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
