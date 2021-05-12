#ifndef MSI_CARCASSONNE_SCOREBOARDVIEW_H
#define MSI_CARCASSONNE_SCOREBOARDVIEW_H
#include <Carcassonne/ScoreBoard.h>
#include <Graphics/Context.h>

namespace carcassonne::frontend {

class ScoreBoardView {
   const ScoreBoard &m_score_board;

 public:
   constexpr explicit ScoreBoardView(const ScoreBoard &score_board) : m_score_board(score_board) {}

   void render(const graphics::Context &ctx) const noexcept;
};

}// namespace carcassonne::frontend

#endif//MSI_CARCASSONNE_SCOREBOARDVIEW_H
