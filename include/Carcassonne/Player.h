#ifndef MSI_CARCASSONNE_PLAYER_H
#define MSI_CARCASSONNE_PLAYER_H

namespace carcassonne {

enum class Player : mb::u8 {
   Black = 0,
   Blue = 1,
   Yellow = 2,
   Red = 3,
};

enum class PlayerAssignment : mb::u8 {
   None = 0,
   Black = 1 << 0,
   Blue = 1 << 1,
   Yellow = 1 << 2,
   Red = 1 << 3,
};

[[nodiscard]] constexpr PlayerAssignment player_to_assignment(Player p) {
   return static_cast<PlayerAssignment>(1 << static_cast<mb::u8>(p));
}

[[nodiscard]] constexpr PlayerAssignment operator&(PlayerAssignment left, PlayerAssignment right) {
   return static_cast<PlayerAssignment>(static_cast<mb::u32>(left) & static_cast<mb::u32>(right));
}

[[nodiscard]] constexpr PlayerAssignment operator|(PlayerAssignment left, PlayerAssignment right) {
   return static_cast<PlayerAssignment>(static_cast<mb::u32>(left) | static_cast<mb::u32>(right));
}

[[nodiscard]] constexpr bool operator&(PlayerAssignment left, Player right) {
   return (left & player_to_assignment(right)) != PlayerAssignment::None;
}

constexpr PlayerAssignment &operator|=(PlayerAssignment &left, PlayerAssignment right) {
   left = left | right;
   return left;
}

constexpr PlayerAssignment &operator&=(PlayerAssignment &left, PlayerAssignment right) {
   left = left & right;
   return left;
}

constexpr PlayerAssignment &operator|=(PlayerAssignment &left, Player right) {
   left = left | player_to_assignment(right);
   return left;
}

[[nodiscard]] constexpr PlayerAssignment operator-(PlayerAssignment left, PlayerAssignment right) {
   return static_cast<PlayerAssignment>(static_cast<mb::u64>(left) & (~static_cast<mb::u64>(right)));
}

[[nodiscard]] std::tuple<PlayerAssignment, Player> constexpr read_player_assignment(PlayerAssignment assignment) {
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

}// namespace carcassonne

#endif//MSI_CARCASSONNE_PLAYER_H
