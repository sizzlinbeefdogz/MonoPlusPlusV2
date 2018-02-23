#include <iostream>
#include "Gamestate.h"
#include "Move.h"


using namespace monopoly;

    int main(int argc, char **argv) {
        Gamestate game_state = setup_game(argv[1], argv[2], argv[3]);

        Move move;

        while (!is_game_over(&game_state)) {
            do {
                display_board(&game_state);

                move = get_move(&game_state);

                make_move(&game_state, &move);

            } while (!move.move_ended);
        }

        display_board(&game_state);

        declare_winners(&game_state);

        return 0;
    }