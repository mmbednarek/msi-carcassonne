#ifndef MSI_CARCASSONNE_CAMERA_H
#define MSI_CARCASSONNE_CAMERA_H
#include <Carcassonne/Core.h>

namespace carcassonne::frontend {

class Camera {
   IBoard &m_board;
   double m_scale = 96.0;
   double m_x = 67.0;
   double m_y = 67.0;
public:
  constexpr explicit Camera(IBoard &board) : m_board(board) {}

   constexpr void scale_up(double amount) {
      if (amount > 0.0 && m_scale > 256.0)
         return;
      if (amount < 0.0 && m_scale < 36.0)
         return;
      m_scale += amount;
   }

   constexpr void move(double x, double y) {
      if (x < 0.0 && m_x < (m_board.min_x() - 8))
         return;
      if (y < 0.0 && m_y < (m_board.min_y() - 8))
         return;
      if (x > 0.0 && m_x > m_board.max_x())
         return;
      if (y > 0.0 && m_y > m_board.max_y())
         return;

      m_x += x / m_scale;
      m_y += y / m_scale;
   }

   [[nodiscard]] constexpr double translate_x(double x) const {
      return m_scale * (x - m_x);
   }

   [[nodiscard]] constexpr double translate_y(double y) const {
      return m_scale * (y - m_y);
   }

   [[nodiscard]] constexpr double scale(double v) const {
      return m_scale * v;
   }

   [[nodiscard]] constexpr double reverse_x(double x) const {
      return x / m_scale + m_x;
   }

   [[nodiscard]] constexpr double reverse_y(double y) const {
      return y / m_scale + m_y;
   }
};

}

#endif//MSI_CARCASSONNE_CAMERA_H
