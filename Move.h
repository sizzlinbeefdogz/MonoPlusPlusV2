#ifndef CPLUSHOARDING_MOVE_H
#define CPLUSHOARDING_MOVE_H

#include "Gamestate.h"

namespace monopoly {
    class Move {
    public:
        int action_id;
        bool move_ended;
        bool dice_rolled;
    };

    Move get_move(Gamestate *game_state);
    void make_move(Gamestate *game_state, Move *move);
}



#endif //CPLUSHOARDING_MOVE_H
