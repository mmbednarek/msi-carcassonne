#include <Carcassonne/Decoder/Decoder.h>
#include <Carcassonne/IGame.h>
// #define SPDLOG_FMT_EXTERNAL
#include <spdlog/spdlog.h>

namespace carcassonne::decoder {

static bool should_skip_iter(Direction d) {
   switch (d) {
   case Direction::NorthEast:
   case Direction::EastSouth:
   case Direction::SouthWest:
   case Direction::WestNorth:
      return true;
   }
   return false;
}

static float fill_allowed_vec(const std::unique_ptr<IGame> &game, std::vector<bool> &allowed_moves, const std::span<float> blob) {
   auto &tile = game->tile_set()[game->move_index()];
   auto sum_neurons = 0.0f;
   auto allowed_it = allowed_moves.begin();
   auto blob_it = blob.begin();

   for (int y = 0; y < g_board_width; ++y) {
      for (int x = 0; x < g_board_height; ++x) {
         for (mb::u8 rot = 0; rot < 4; ++rot) {
            if (!game->board().can_place_at(x, y, tile, rot)) {
               for (int i = 0; i < 14; ++i) {
                  *allowed_it = false;
                  ++allowed_it;
               }
               for (int i = 0; i < 10; ++i) {
                  ++blob_it;
               }
               continue;
            }

            for (auto dir : g_directions) {
               if (!game->can_place_tile_and_figure(x, y, rot, tile, dir)) {
                  *allowed_it = false;
                  ++allowed_it;
                  if (!should_skip_iter(dir)) {
                     ++blob_it;
                  }
                  continue;
               }

               *allowed_it = true;
               sum_neurons += *blob_it;
               ++allowed_it;
               if (!should_skip_iter(dir)) {
                  ++blob_it;
               }
            }

            *allowed_it = true;
            sum_neurons += *blob_it;
            ++blob_it;
            ++allowed_it;
         }
      }
   }
   if (allowed_it != allowed_moves.begin() + allowed_moves.size()) {
      spdlog::debug("fill_allowed_vec, allowed_it: should never happen!");
   }
   if (blob_it != blob.begin() + blob.size()) {
      spdlog::debug("fill_allowed_vec, blob_it: should never happen!");
   }

   return sum_neurons;
}

FullMove decode_move(const std::unique_ptr<IGame> &game, std::vector<bool> &allowed_moves, const std::span<float> blob, float prob) {
   using mb::size, mb::u8;
   auto sum_neurons = fill_allowed_vec(game, allowed_moves, blob);
   auto threshold = sum_neurons * prob;

   auto allowed_it = allowed_moves.begin();
   auto blob_it = blob.begin();
   auto sum = 0.0f;

   for (int y = 0; y < g_board_width; ++y) {
      for (int x = 0; x < g_board_height; ++x) {
         for (mb::u8 rot = 0; rot < 4; ++rot) {
            for (auto dir : g_directions) {
               if (*allowed_it) {
                  sum += *blob_it;
                  if (sum >= threshold) {
                     // spdlog::info("neuron_id={} of {}", blob_it - blob.begin(), blob.size());
                     return FullMove{
                             .x = x,
                             .y = y,
                             .rotation = rot,
                             .ignored_figure = false,
                             .direction = dir,
                     };
                  }
               }

               ++allowed_it;
               if (!should_skip_iter(dir)) {
                  ++blob_it;
               }
            }

            if (*allowed_it) {
               sum += *blob_it;
               if (sum >= threshold) {
                  // spdlog::info("neuron_id={} of {}", blob_it - blob.begin(), blob.size());
                  return FullMove{
                          .x = x,
                          .y = y,
                          .rotation = rot,
                          .ignored_figure = true,
                  };
               }
            }
            ++blob_it;
            ++allowed_it;
         }
      }
   }
   spdlog::error("decode: should never happen!");
   return FullMove{};
}

}// namespace carcassonne::decoder
