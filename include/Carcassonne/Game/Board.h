#ifndef MSI_CARCASSONNE_BOARD_H
#define MSI_CARCASSONNE_BOARD_H
#include <Carcassonne/Core.h>
#include <mb/int.h>

namespace carcassonne::game {

class Board : public IBoard {
   std::array<TilePlacement, g_board_width * g_board_height> m_board{};
   int m_min_x = 70;
   int m_min_y = 70;
   int m_max_x = 71;
   int m_max_y = 71;

 public:
   [[nodiscard]] int min_x() const noexcept override;
   [[nodiscard]] int min_y() const noexcept override;
   [[nodiscard]] int max_x() const noexcept override;
   [[nodiscard]] int max_y() const noexcept override;
   [[nodiscard]] TilePlacement tile_at(int x, int y) const noexcept override;
   [[nodiscard]] bool can_place_at(int x, int y, TileType t, mb::u8 rotation) const noexcept override;
   void set_tile(int x, int y, TileType t, mb::u8 rotation) noexcept override;
};

}// namespace carcassonne::game

#endif//MSI_CARCASSONNE_BOARD_H
