#ifndef MSI_CARCASSONNE_CONTACT_H
#define MSI_CARCASSONNE_CONTACT_H
#include <mb/int.h>

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

}// namespace carcassonne

#endif//MSI_CARCASSONNE_CONTACT_H
