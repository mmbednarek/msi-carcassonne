#ifndef MSI_CARCASSONNE_TRAINING_TRAININGSET
#define MSI_CARCASSONNE_TRAINING_TRAININGSET
#include <Carcassonne/Player.h>
#include <vector>
#include <functional>
#include <mb/int.h>

namespace carcassonne::training {

struct MoveNetworkRecord {
   std::vector<float> game_state;
   std::array<float, 67240> children_visits;
   float reward;
   Player player;
};

using OneGame = std::vector<MoveNetworkRecord>;

struct TrainingSet {
   std::vector<OneGame> games;
};

} // namespace carcassonne::training

#endif // MSI_CARCASSONNE_TRAINING_TRAININGSET