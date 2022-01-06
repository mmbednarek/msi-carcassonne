#ifndef MSI_CARCASSONNE_TRAINING_TRAININGSET
#define MSI_CARCASSONNE_TRAINING_TRAININGSET
#include <vector>
#include <functional>
#include <mb/int.h>

namespace carcassonne::training {

struct MoveNetworkRecord {
   std::vector<float>& game_state;
   std::vector<std::reference_wrapper<mb::size>> children_visits;
   float reward;
};

using OneGame = std::vector<MoveNetworkRecord>;

struct TrainingSet {
   std::vector<OneGame> games;
};

} // namespace carcassonne::training

#endif // MSI_CARCASSONNE_TRAINING_TRAININGSET