#ifndef MSI_CARCASSONNE_MOVEITERATOR_H
#define MSI_CARCASSONNE_MOVEITERATOR_H
#include "IBoard.h"
#include "Player.h"
#include <mb/result.h>
#include <memory>

namespace carcassonne {

struct TileMove {
   int x, y;
   mb::u8 rotation;

   constexpr bool operator==(const TileMove &other) const {
      return x == other.x && y == other.y && rotation == other.rotation;
   }

   constexpr bool operator!=(const TileMove &other) const {
      return x != other.x || y != other.y || rotation != other.rotation;
   }
};

struct FullMove {
   int x{}, y{};
   mb::u8 rotation{};
   bool ignored_figure = true;
   Direction direction{};

   constexpr bool operator==(const FullMove &other) const {
      return x == other.x &&
             y == other.y &&
             rotation == other.rotation &&
             ignored_figure == other.ignored_figure &&
             direction == other.direction;
   }
};

constexpr std::size_t encode_direction(Direction dir, bool ignored_figure) {
   if (ignored_figure)
      return 9;

   switch (dir) {
   case Direction::North: return 0;
   case Direction::East: return 1;
   case Direction::South: return 2;
   case Direction::West: return 3;
   case Direction::Middle: return 4;
   case Direction::NorthEast:
   case Direction::EastNorth: return 5;
   case Direction::EastSouth:
   case Direction::SouthEast: return 6;
   case Direction::SouthWest:
   case Direction::WestSouth: return 7;
   case Direction::WestNorth:
   case Direction::NorthWest: return 8;
   }
   return 0;
}

constexpr std::size_t encode_move(const FullMove &move) {
   return g_board_width * 4 * 10 * move.y + 4 * 10 * move.x +  10 * move.rotation + encode_direction(move.direction, move.ignored_figure);
}

class MoveIter {
   TileMove m_move;
   TileType m_tile_type;
   std::reference_wrapper<const IBoard> m_board;

 public:
   typedef MoveIter self_type;
   typedef TileMove value_type;
   typedef TileMove &reference;
   typedef const TileMove &const_reference;
   typedef TileMove *pointer;
   typedef const TileMove *const_pointer;
   typedef std::bidirectional_iterator_tag iterator_category;
   typedef mb::size difference_type;

   inline explicit MoveIter(const IBoard &board, TileType tt) : m_move{
                                                                           .x = board.min_x() - 1,
                                                                           .y = board.min_y() - 1,
                                                                           .rotation = 0,
                                                                   },
                                                                   m_tile_type(tt), m_board(board) {
      seek_next();
   }

   inline explicit MoveIter(const IBoard &board, TileType tt, int x, int y, mb::u8 rotation) : m_move{
                                                                                                          .x = x,
                                                                                                          .y = y,
                                                                                                          .rotation = rotation,
                                                                                                  },
                                                                                                  m_tile_type(tt), m_board(board) {
   }

   inline self_type operator++() {
      self_type tmp_it(*this);
      progress();
      seek_next();
      return tmp_it;
   }

   inline self_type operator++(int) {
      progress();
      seek_next();
      return *this;
   }

   inline self_type operator--() {
      self_type tmp_it(*this);
      regress();
      seek_prev();
      return tmp_it;
   }

   inline self_type operator--(int) {
      regress();
      seek_prev();
      return *this;
   }

   inline self_type operator+(difference_type count) const {
      self_type result(*this);
      for (difference_type i = 0; i < count; ++i) {
         result.progress();
         result.seek_next();
      }
      return result;
   }

   inline self_type operator-(difference_type count) const {
      self_type result(*this);
      for (difference_type i = 0; i < count; ++i) {
         result.regress();
         result.seek_prev();
      }
      return result;
   }

   inline difference_type operator-(const self_type &other) const {
      difference_type count{};
      for (self_type at = other; at != *this; ++at) {
         ++count;
      }
      return count;
   }

   constexpr reference operator*() {
      return m_move;
   }

   constexpr const_pointer operator->() const {
      return &m_move;
   }

   constexpr bool operator==(const self_type &other) const {
      return m_move == other.m_move;
   }

   constexpr bool operator!=(const self_type &other) const {
      return m_move != other.m_move;
   }

 private:
   constexpr bool progress() {
      if (m_move.rotation < 3) {
         ++m_move.rotation;
         return true;
      }
      m_move.rotation = 0;

      if (m_move.x < m_board.get().max_x()) {
         ++m_move.x;
         return true;
      }
      m_move.x = m_board.get().min_x() - 1;

      if (m_move.y < m_board.get().max_y()) {
         ++m_move.y;
         return true;
      }

      m_move.y = m_board.get().max_y() + 1;
      return false;
   }

   constexpr bool regress() {
      if (m_move.rotation > 0) {
         --m_move.rotation;
         return true;
      }
      m_move.rotation = 3;

      if (m_move.x >= m_board.get().min_x()) {
         --m_move.x;
         return true;
      }
      m_move.x = m_board.get().max_x();

      if (m_move.y >= m_board.get().min_y()) {
         --m_move.y;
         return true;
      }

      m_move.y = m_board.get().min_y() - 1;
      return false;
   }

   inline void seek_prev() {
      do {
         if (m_board.get().can_place_at(m_move.x, m_move.y, m_tile_type, m_move.rotation))
            return;
      } while (regress());
   }

   inline void seek_next() {
      do {
         if (m_board.get().can_place_at(m_move.x, m_move.y, m_tile_type, m_move.rotation))
            return;
      } while (progress());
   }
};

struct MoveRange {
   MoveIter from;
   MoveIter to;

   [[nodiscard]] inline MoveIter begin() const {
      return from;
   }

   [[nodiscard]] inline MoveIter end() const {
      return to;
   }
};

enum class MovePhase {
   PlaceTile,
   PlaceFigure,
   Done
};

class IGame;

class IMove {
 public:
   [[nodiscard]] virtual Player player() const noexcept = 0;
   [[nodiscard]] virtual TileType tile_type() const noexcept = 0;
   [[nodiscard]] virtual MovePhase phase() const noexcept = 0;
   [[nodiscard]] virtual bool is_free(Direction d) const noexcept = 0;
   [[nodiscard]] virtual TilePosition position() const noexcept = 0;
   [[nodiscard]] virtual std::unique_ptr<IMove> clone(IGame &game) const noexcept = 0;
   virtual mb::result<mb::empty> place_tile_at(int x, int y, mb::u8 rotation) noexcept = 0;
   virtual mb::result<mb::empty> place_tile(TileMove tile_location) noexcept = 0;
   virtual mb::result<mb::empty> place_figure(Direction d) noexcept = 0;
   virtual mb::result<mb::empty> ignore_figure() noexcept = 0;
};

}// namespace carcassonne

#endif//MSI_CARCASSONNE_MOVE_H
