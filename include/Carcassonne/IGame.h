#ifndef MSI_CARCASSONNE_IGAME_H
#define MSI_CARCASSONNE_IGAME_H
#include "IBoard.h"
#include "Direction.h"
#include "Edge.h"
#include "Move.h"
#include "Player.h"
#include "ScoreBoard.h"
#include <mb/view.h>
#include <memory>

namespace carcassonne {

struct Figure {
   Player player;
   double x, y;
   int tile_x, tile_y;
   Edge edge;
};

class IGame {
 public:
   virtual ~IGame() = default;
   virtual std::unique_ptr<IGame> clone() const noexcept = 0;
   [[nodiscard]] virtual const IBoard &board() const noexcept = 0;
   [[nodiscard]] virtual Player current_player() const noexcept = 0;
   [[nodiscard]] virtual constexpr const mb::size &player_count() const noexcept = 0;
   [[nodiscard]] virtual std::unique_ptr<IMove> new_move(Player p) noexcept = 0;
   [[nodiscard]] virtual mb::view<Figure> figures() const noexcept = 0;
   [[nodiscard]] virtual const ScoreBoard &scores() const noexcept = 0;
   [[nodiscard]] virtual mb::u8 move_index() const noexcept = 0;
   [[nodiscard]] virtual const std::vector<TileType> &tile_set() const noexcept = 0;
   [[nodiscard]] virtual std::vector<Direction> figure_placements(int x, int y) const noexcept = 0;
   virtual void start() noexcept = 0;
   virtual void on_next_move(std::function<void(IGame &, Player)> callback) noexcept = 0;
   virtual void update(double dt) noexcept = 0;

   [[nodiscard]] inline MoveRange moves(TileType tile_type = 0) const {
      if (tile_type == 0) {
         tile_type = tile_set()[move_index()];
      }
      return MoveRange{
              .from = MoveIter(board(), tile_type),
              .to = MoveIter(board(), tile_type, board().min_x() - 1, board().max_y() + 1, 0),
      };
   }
};

}// namespace carcassonne

#endif//MSI_CARCASSONNE_IGAME_H