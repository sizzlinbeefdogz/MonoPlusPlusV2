#ifndef CPLUSHOARDING_GAMESTATE_H
#define CPLUSHOARDING_GAMESTATE_H

#include <string>
#include "Board.h"
#include "Rules.h"
#include "Player.h"

namespace monopoly {
    class Gamestate {
    public:
        Board board;
        int num_players;
        std::vector<Player> players;
        int player_turn_id;
        bool player_turn_made_move;
        Rules rules;
        int num_moves;
        int random_file_index;

        bool should_re_roll = false;
        int num_current_re_rolls = 0;

        Player current_player() {
            return this->players.at(this->player_turn_id);
        }
    };

    Gamestate setup_game(std::string rule_dir, std::string board_dir, std::string rand_dir);
    bool is_game_over(Gamestate *game_state);
    void declare_winners(Gamestate *gm);
}

#endif //CPLUSHOARDING_GAMESTATE_H
