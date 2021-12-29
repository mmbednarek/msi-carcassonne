#ifndef CARCASSONNE_GPU_GAME_MOVE_H
#define CARCASSONNE_GPU_GAME_MOVE_H
#ifdef __CUDACC__
#define DEVHOST __device__ __host__
#else
#define DEVHOST
#endif

#include "Board.h"
#include <Carcassonne_GPU/Player.h>
#include <mb/int.h>
#include <ecuda/ptr/unique_ptr.hpp>

namespace carcassonne::game {

struct TileMove {
   int x;
   int y;
   mb::u8 rotation;

   constexpr bool operator==(const TileMove &other) const {
      return x == other.x && y == other.y && rotation == other.rotation;
   }

   constexpr bool operator!=(const TileMove &other) const {
      return x != other.x || y != other.y || rotation != other.rotation;
   }
};

struct FullMove {
   int x{};
   int y{};
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

class MoveIter {
   TileMove m_move;
   TileType m_tile_type;
   const Board &m_board;

 public:
   typedef MoveIter self_type;
   typedef TileMove value_type;
   typedef TileMove &reference;
   typedef const TileMove &const_reference;
   typedef TileMove *pointer;
   typedef const TileMove *const_pointer;
   typedef std::bidirectional_iterator_tag iterator_category;
   typedef mb::size difference_type;

   inline explicit MoveIter(const Board &board, TileType tt)
    : m_move{/*.x =*/ board.min_x() - 1, /*.y =*/ board.min_y() - 1, /*.rotation =*/ 0}
    , m_tile_type(tt)
    , m_board(board) {
      seek_next();
   }

   inline explicit MoveIter(const Board &board, TileType tt, int x, int y, mb::u8 rotation)
    : m_move{/*.x =*/ x, /*.y =*/ y, /*.rotation =*/ rotation}
    , m_tile_type(tt)
    , m_board(board) {
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

      if (m_move.x < m_board.max_x()) {
         ++m_move.x;
         return true;
      }
      m_move.x = m_board.min_x() - 1;

      if (m_move.y < m_board.max_y()) {
         ++m_move.y;
         return true;
      }

      m_move.y = m_board.max_y() + 1;
      return false;
   }

   constexpr bool regress() {
      if (m_move.rotation > 0) {
         --m_move.rotation;
         return true;
      }
      m_move.rotation = 3;

      if (m_move.x >= m_board.min_x()) {
         --m_move.x;
         return true;
      }
      m_move.x = m_board.max_x();

      if (m_move.y >= m_board.min_y()) {
         --m_move.y;
         return true;
      }

      m_move.y = m_board.min_y() - 1;
      return false;
   }

   inline void seek_prev() {
      do {
         if (m_board.can_place_at(m_move.x, m_move.y, m_tile_type, m_move.rotation))
            return;
      } while (regress());
   }

   inline void seek_next() {
      do {
         if (m_board.can_place_at(m_move.x, m_move.y, m_tile_type, m_move.rotation))
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

class Game;

class Move {
   Game &m_game;
   Player m_player;
   TileType m_tile_type;
   MovePhase m_phase = MovePhase::PlaceTile;
   int m_x{}, m_y{};
   mb::u8 m_rotation{};

 public:
   constexpr Move(Player p, TileType t, Game &game) : m_game(game), m_player(p), m_tile_type(t) {}

   constexpr Move(Game &game, Player player, TileType tile_type, int x, int y) : m_game(game),
                                                                                 m_player(player),
                                                                                 m_tile_type(tile_type),
                                                                                 m_x(x),
                                                                                 m_y(y),
                                                                                 m_phase(MovePhase::PlaceFigure) {}

   [[nodiscard]] Player player() const noexcept;
   [[nodiscard]] TileType tile_type() const noexcept;
   [[nodiscard]] MovePhase phase() const noexcept;
   [[nodiscard]] TilePosition position() const noexcept;
   [[nodiscard]] bool is_free(Direction d) const noexcept;
   bool place_tile(TileMove tile_location) noexcept;
   bool place_tile_at(int x, int y, mb::u8 rotation) noexcept;
   bool place_figure(Direction d) noexcept;
   bool ignore_figure() noexcept;
   ecuda::unique_ptr<Move> clone(Game &game) const noexcept;
};

}// namespace carcassonne::game

#endif//CARCASSONNE_GPU_GAME_MOVE_H
