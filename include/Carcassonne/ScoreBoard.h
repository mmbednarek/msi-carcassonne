#ifndef MSI_CARCASSONNE_SCOREBOARD_H
#define MSI_CARCASSONNE_SCOREBOARD_H
#include "Core.h"
#include <set>

namespace carcassonne {

struct PlayerScore {
   Player player{};
   short score{};

   constexpr PlayerScore(Player player, short score) : player(player), score(score) {}
};

class ScoreBoard {
   std::vector<PlayerScore> m_score{};

 public:
   inline void add_points(Player p, short points) {
      auto it = std::find_if(m_score.begin(), m_score.end(),
                             [p](PlayerScore score) { return score.player == p; });
      if (it == m_score.end()) {
         m_score.emplace_back(p, points);
         return;
      }

      it->score += points;
      std::sort(m_score.begin(), m_score.end(), [](const PlayerScore lhs, const PlayerScore rhs) { return lhs.score > rhs.score; });
   }

   inline void add_points(PlayerAssignment assignment, short points) {
      Player player;
      while (assignment != PlayerAssignment::None) {
         std::tie(assignment, player) = read_player_assignment(assignment);
         add_points(player, points);
      }
   }

   [[nodiscard]] inline std::vector<PlayerScore>::const_iterator begin() const {
      return m_score.cbegin();
   }

   [[nodiscard]] inline std::vector<PlayerScore>::const_iterator end() const {
      return m_score.cend();
   }
};

}// namespace carcassonne

#endif//MSI_CARCASSONNE_SCOREBOARD_H
