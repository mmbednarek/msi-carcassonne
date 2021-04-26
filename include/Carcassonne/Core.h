#ifndef MSI_CARCASSONNE_CORE_H
#define MSI_CARCASSONNE_CORE_H
#include <array>
#include <mb/int.h>

namespace carcassonne {

struct Position {
   mb::f64 x, y;
};

enum class EdgeType {
   Grass,
   Path,
   Town
};

enum class Connection : mb::u32 {
   None = 0,
   NorthEast = 1 << 0,
   NorthSouth = 1 << 1,
   NorthWest = 1 << 2,
   SouthEast = 1 << 3,
   SouthWest = 1 << 4,
   WestEast = 1 << 5,
   All = 0x3f,
};

constexpr Connection operator|(Connection left, Connection right) {
   return static_cast<Connection>(static_cast<mb::u32>(left) | static_cast<mb::u32>(right));
}

struct Tile {
   std::array<EdgeType, 4> edges{};
   Connection connections = Connection::None;
   bool monastery = false;
   bool pennant = false;
};

using TileType = mb::u8;

struct TilePlacement {
   TileType type = 0;
   mb::u8 rotation = 0;
};

constexpr std::array<Tile, 25> g_tiles{
        Tile{
                .edges{EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
        },
        {
                .edges{EdgeType::Path, EdgeType::Town, EdgeType::Path, EdgeType::Grass},
                .connections = Connection::NorthWest,
        },
        {
                .edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
        },
        {
                .edges{EdgeType::Town, EdgeType::Grass, EdgeType::Town, EdgeType::Grass},
                .connections = Connection::NorthWest,
        },
        {
                .edges{EdgeType::Grass, EdgeType::Town, EdgeType::Grass, EdgeType::Town},
        },
        {
                .edges{EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
                .monastery = true,
        },
        {
                .edges{EdgeType::Grass, EdgeType::Grass, EdgeType::Path, EdgeType::Grass},
                .monastery = true,
        },
        {
                .edges{EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Grass},
        },
        {
                .edges{EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Town},
                .connections = Connection::All,
                .pennant = true,
        },
        {
                .edges{EdgeType::Town, EdgeType::Path, EdgeType::Path, EdgeType::Grass},
                .connections = Connection::SouthEast,
        },
        {
                .edges{EdgeType::Town, EdgeType::Grass, EdgeType::Path, EdgeType::Path},
                .connections = Connection::SouthWest,
        },
        {
                .edges{EdgeType::Town, EdgeType::Path, EdgeType::Path, EdgeType::Path},
        },
        {
                .edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town},
                .connections = Connection::NorthWest,
                .pennant = true,
        },
        {
                .edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town},
                .connections = Connection::NorthWest,
        },
        {
                .edges{EdgeType::Town, EdgeType::Path, EdgeType::Path, EdgeType::Town},
                .connections = Connection::NorthWest | Connection::SouthEast,
        },
        {
                .edges{EdgeType::Town, EdgeType::Path, EdgeType::Path, EdgeType::Town},
                .connections = Connection::NorthWest | Connection::SouthEast,
                .pennant = true,
        },
        {
                .edges{EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Town},
                .connections = Connection::NorthEast | Connection::NorthWest | Connection::WestEast,
                .pennant = true,
        },
        {
                .edges{EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Town},
                .connections = Connection::NorthEast | Connection::NorthWest | Connection::WestEast,
        },
        {
                .edges{EdgeType::Town, EdgeType::Path, EdgeType::Path, EdgeType::Town},
                .connections = Connection::NorthEast | Connection::NorthWest | Connection::WestEast,
                .pennant = true,
        },
        {
                .edges{EdgeType::Town, EdgeType::Path, EdgeType::Path, EdgeType::Town},
                .connections = Connection::NorthEast | Connection::NorthWest | Connection::WestEast,
        },
        {
                .edges{EdgeType::Path, EdgeType::Grass, EdgeType::Path, EdgeType::Grass},
                .connections = Connection::NorthSouth,
        },
        {
                .edges{EdgeType::Grass, EdgeType::Grass, EdgeType::Path, EdgeType::Path},
                .connections = Connection::SouthWest,
        },
        {
                .edges{EdgeType::Grass, EdgeType::Path, EdgeType::Path, EdgeType::Path},
        },
        {
                .edges{EdgeType::Path, EdgeType::Path, EdgeType::Path, EdgeType::Path},
        },
        {
                .edges{EdgeType::Town, EdgeType::Grass, EdgeType::Town, EdgeType::Grass},
                .connections = Connection::NorthSouth,
                .pennant = true,
        },
};

/*
 The maximal board size is 141x141
 Starting tile is at x = 70, y = 70
 */
class IBoard {
 public:
   [[nodiscard]] virtual int min_x() const noexcept = 0;
   [[nodiscard]] virtual int min_y() const noexcept = 0;
   [[nodiscard]] virtual int max_x() const noexcept = 0;
   [[nodiscard]] virtual int max_y() const noexcept = 0;
   [[nodiscard]] virtual TilePlacement tile_at(int x, int y) const noexcept = 0;
   virtual void set_tile(int x, int y, TileType t, mb::u8 rotation) noexcept = 0;
};

class IGame {
 public:
   [[nodiscard]] virtual const Position &block_pos() const noexcept = 0;
   [[nodiscard]] virtual IBoard &board() noexcept = 0;
   virtual void set_block_pos(mb::f64 x, mb::f64 y) noexcept = 0;
};

}// namespace carcassonne

#endif//MSI_CARCASSONNE_CORE_H
