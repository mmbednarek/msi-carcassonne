#ifndef MSI_CARCASSONNE_CONTACT_H
#define MSI_CARCASSONNE_CONTACT_H
#include <mb/int.h>
#include <tuple>
#include "Direction.h"

namespace carcassonne {

enum class Contact : mb::u64 {
   None = 0,
   NorthNWX = 1 << 0,
   NorthNEX = 1 << 1,
   NorthSWX = 1 << 2,
   NorthSWY = 1 << 3,
   NorthSEX = 1 << 4,
   NorthSEY = 1 << 5,
   EastNEY = 1 << 6,
   EastSEY = 1 << 7,
   EastNWY = 1 << 8,
   EastNWX = 1 << 9,
   EastSWY = 1 << 10,
   EastSWX = 1 << 11,
   SouthSEX = 1 << 12,
   SouthSWX = 1 << 13,
   SouthNEX = 1 << 14,
   SouthNEY = 1 << 15,
   SouthNWX = 1 << 16,
   SouthNWY = 1 << 17,
   WestSWY = 1 << 18,
   WestNWY = 1 << 19,
   WestSEY = 1 << 20,
   WestSEX = 1 << 21,
   WestNEY = 1 << 22,
   WestNEX = 1 << 23,
   North = NorthNWX | NorthNEX | NorthSWX | NorthSWY | NorthSEX | NorthSEY,
   East = EastNEY | EastSEY | EastNWY | EastNWX | EastSWY | EastSWX,
   South = SouthSEX | SouthSWX | SouthNEX | SouthNEY | SouthNWX | SouthNWY,
   West = WestSWY | WestNWY | WestSEY | WestSEX | WestNEY | WestNEX,
   NorthW = NorthNWX | NorthSWX,
   NorthE = NorthNEX | NorthSEX,
   NorthS = NorthSWY | NorthSEY,
   EastN = EastNEY | EastNWY,
   EastS = EastSEY | EastSWY,
   EastW = EastNWX | EastSWX,
   SouthW = SouthSWX | SouthNWX,
   SouthE = SouthSEX | SouthNEX,
   SouthN = SouthNWY | SouthNEY,
   WestS = WestSWY | WestSEY,
   WestN = WestNWY | WestNEY,
   WestE = WestNEX | WestSEX,
};

[[nodiscard]] constexpr Contact operator|(Contact left, Contact right) {
   return static_cast<Contact>(static_cast<mb::u64>(left) | static_cast<mb::u64>(right));
}

constexpr Contact &operator|=(Contact &left, Contact right) {
   left = left | right;
   return left;
}

[[nodiscard]] constexpr bool operator&(Contact left, Contact right) {
   return (static_cast<mb::u64>(left) & static_cast<mb::u64>(right)) != 0;
}

[[nodiscard]] constexpr Contact operator-(Contact left, Contact right) {
   return static_cast<Contact>(static_cast<mb::u64>(left) & (~static_cast<mb::u64>(right)));
}

[[nodiscard]] constexpr Contact rotate_contact(Contact c) {
   Contact result{};

   if (c & Contact::NorthNWX)
      result |= Contact::EastNEY;
   if (c & Contact::EastNEY)
      result |= Contact::SouthSEX;
   if (c & Contact::SouthSEX)
      result |= Contact::WestSWY;
   if (c & Contact::WestSWY)
      result |= Contact::NorthNWX;

   if (c & Contact::NorthNEX)
      result |= Contact::EastSEY;
   if (c & Contact::EastSEY)
      result |= Contact::SouthSWX;
   if (c & Contact::SouthSWX)
      result |= Contact::WestNWY;
   if (c & Contact::WestNWY)
      result |= Contact::NorthNEX;

   if (c & Contact::NorthSWX)
      result |= Contact::EastNWY;
   if (c & Contact::EastNWY)
      result |= Contact::SouthNEX;
   if (c & Contact::SouthNEX)
      result |= Contact::WestSEY;
   if (c & Contact::WestSEY)
      result |= Contact::NorthSWX;

   if (c & Contact::NorthSWY)
      result |= Contact::EastNWX;
   if (c & Contact::EastNWX)
      result |= Contact::SouthNEY;
   if (c & Contact::SouthNEY)
      result |= Contact::WestSEX;
   if (c & Contact::WestSEX)
      result |= Contact::NorthSWY;

   if (c & Contact::NorthSEX)
      result |= Contact::EastSWY;
   if (c & Contact::EastSWY)
      result |= Contact::SouthNWX;
   if (c & Contact::SouthNWX)
      result |= Contact::WestNEY;
   if (c & Contact::WestNEY)
      result |= Contact::NorthSEX;

   if (c & Contact::NorthSEY)
      result |= Contact::EastSWX;
   if (c & Contact::EastSWX)
      result |= Contact::SouthNWY;
   if (c & Contact::SouthNWY)
      result |= Contact::WestNEX;
   if (c & Contact::WestNEX)
      result |= Contact::NorthSEY;

   return result;
}

[[nodiscard]] constexpr std::tuple<Contact, Direction, Direction> read_contacts(Contact c) {
   if (c & Contact::NorthNWX)
      return std::make_tuple(c - Contact::NorthNWX, Direction::North, Direction::WestNorth);
   if (c & Contact::NorthNEX)
      return std::make_tuple(c - Contact::NorthNEX, Direction::North, Direction::EastNorth);
   if (c & Contact::NorthSWX)
      return std::make_tuple(c - Contact::NorthSWX, Direction::North, Direction::WestSouth);
   if (c & Contact::NorthSWY)
      return std::make_tuple(c - Contact::NorthSWY, Direction::North, Direction::SouthWest);
   if (c & Contact::NorthSEX)
      return std::make_tuple(c - Contact::NorthSEX, Direction::North, Direction::EastSouth);
   if (c & Contact::NorthSEY)
      return std::make_tuple(c - Contact::NorthSEY, Direction::North, Direction::SouthEast);

   if (c & Contact::EastNEY)
      return std::make_tuple(c - Contact::EastNEY, Direction::East, Direction::NorthEast);
   if (c & Contact::EastSEY)
      return std::make_tuple(c - Contact::EastSEY, Direction::East, Direction::SouthEast);
   if (c & Contact::EastNWY)
      return std::make_tuple(c - Contact::EastNWY, Direction::East, Direction::NorthWest);
   if (c & Contact::EastNWX)
      return std::make_tuple(c - Contact::EastNWX, Direction::East, Direction::WestNorth);
   if (c & Contact::EastSWY)
      return std::make_tuple(c - Contact::EastSWY, Direction::East, Direction::SouthWest);
   if (c & Contact::EastSWX)
      return std::make_tuple(c - Contact::EastSWX, Direction::East, Direction::WestSouth);

   if (c & Contact::SouthSEX)
      return std::make_tuple(c - Contact::SouthSEX, Direction::South, Direction::EastSouth);
   if (c & Contact::SouthSWX)
      return std::make_tuple(c - Contact::SouthSWX, Direction::South, Direction::WestSouth);
   if (c & Contact::SouthNEX)
      return std::make_tuple(c - Contact::SouthNEX, Direction::South, Direction::EastNorth);
   if (c & Contact::SouthNEY)
      return std::make_tuple(c - Contact::SouthNEY, Direction::South, Direction::NorthEast);
   if (c & Contact::SouthNWX)
      return std::make_tuple(c - Contact::SouthNWX, Direction::South, Direction::WestNorth);
   if (c & Contact::SouthNWY)
      return std::make_tuple(c - Contact::SouthNWY, Direction::South, Direction::NorthWest);

   if (c & Contact::WestSWY)
      return std::make_tuple(c - Contact::WestSWY, Direction::South, Direction::SouthWest);
   if (c & Contact::WestNWY)
      return std::make_tuple(c - Contact::WestNWY, Direction::South, Direction::NorthWest);
   if (c & Contact::WestSEY)
      return std::make_tuple(c - Contact::WestSEY, Direction::South, Direction::SouthEast);
   if (c & Contact::WestSEX)
      return std::make_tuple(c - Contact::WestSEX, Direction::South, Direction::EastSouth);
   if (c & Contact::WestNEY)
      return std::make_tuple(c - Contact::WestNEY, Direction::South, Direction::NorthEast);
   if (c & Contact::WestNEX)
      return std::make_tuple(c - Contact::WestNEX, Direction::South, Direction::EastNorth);

   return std::make_tuple(Contact::None, Direction::Middle, Direction::Middle);
}

}// namespace carcassonne

#endif//MSI_CARCASSONNE_CONTACT_H
