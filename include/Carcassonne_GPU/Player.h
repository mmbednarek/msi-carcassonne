#ifndef CARCASSONNE_GPU_PLAYER_H
#define CARCASSONNE_GPU_PLAYER_H
#ifdef __CUDACC__
#define DEVHOST __device__ __host__
#else
#define DEVHOST
#endif

#include <array>
#include <tuple>
#include <mb/int.h>

namespace carcassonne {

enum class Player : mb::u8 {
   Black = 0,
   Blue = 1,
   Yellow = 2,
   Red = 3,
};

DEVHOST constexpr std::array<Player, 4> g_players{
        Player::Black,
        Player::Blue,
        Player::Yellow,
        Player::Red,
};

enum class PlayerAssignment : mb::u8 {
   None = 0,
   Black = 1 << 0,
   Blue = 1 << 1,
   Yellow = 1 << 2,
   Red = 1 << 3,
};

[[nodiscard]] DEVHOST constexpr PlayerAssignment player_to_assignment(Player p) {
   return static_cast<PlayerAssignment>(1 << static_cast<mb::u8>(p));
}

[[nodiscard]] DEVHOST constexpr PlayerAssignment operator&(PlayerAssignment left, PlayerAssignment right) {
   return static_cast<PlayerAssignment>(static_cast<mb::u32>(left) & static_cast<mb::u32>(right));
}

[[nodiscard]] DEVHOST constexpr PlayerAssignment operator|(PlayerAssignment left, PlayerAssignment right) {
   return static_cast<PlayerAssignment>(static_cast<mb::u32>(left) | static_cast<mb::u32>(right));
}

[[nodiscard]] DEVHOST constexpr bool operator&(PlayerAssignment left, Player right) {
   return (left & player_to_assignment(right)) != PlayerAssignment::None;
}

DEVHOST constexpr PlayerAssignment &operator|=(PlayerAssignment &left, PlayerAssignment right) {
   left = left | right;
   return left;
}

DEVHOST constexpr PlayerAssignment &operator&=(PlayerAssignment &left, PlayerAssignment right) {
   left = left & right;
   return left;
}

DEVHOST constexpr PlayerAssignment &operator|=(PlayerAssignment &left, Player right) {
   left = left | player_to_assignment(right);
   return left;
}

[[nodiscard]] DEVHOST constexpr PlayerAssignment operator-(PlayerAssignment left, PlayerAssignment right) {
   return static_cast<PlayerAssignment>(static_cast<mb::u64>(left) & (~static_cast<mb::u64>(right)));
}

[[nodiscard]] DEVHOST std::tuple<PlayerAssignment, Player> constexpr read_player_assignment(PlayerAssignment assignment) {
   if (assignment & Player::Black)
      return std::make_tuple(assignment - PlayerAssignment::Black, Player::Black);
   if (assignment & Player::Blue)
      return std::make_tuple(assignment - PlayerAssignment::Blue, Player::Blue);
   if (assignment & Player::Yellow)
      return std::make_tuple(assignment - PlayerAssignment::Yellow, Player::Yellow);
   if (assignment & Player::Red)
      return std::make_tuple(assignment - PlayerAssignment::Red, Player::Red);

   return std::make_tuple(PlayerAssignment::None, Player::Black);
}

constexpr Player DEVHOST next_player(Player player, mb::u8 player_count) {
   return static_cast<Player>((static_cast<int>(player) + 1) % player_count);
}

constexpr Player DEVHOST last_player(Player player, mb::u8 player_count) {
   return static_cast<Player>((static_cast<int>(player) + player_count - 1) % player_count);
}

}// namespace carcassonne

#endif//CARCASSONNE_GPU_PLAYER_H
