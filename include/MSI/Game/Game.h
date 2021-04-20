#ifndef MSI_CARCASSONNE_GAME_H
#define MSI_CARCASSONNE_GAME_H
#include <MSI/Graphics/Graphics.h>

namespace msi::game {

class Game {
   bool m_running = true;
 public:
   void run();
};

}

#endif //MSI_CARCASSONNE_GAME_H
