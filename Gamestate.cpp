#include "Gamestate.h"
#include <string>
#include "Rules.h"
#include "Board.h"
#include "Player.h"

namespace monopoly {

    Gamestate create_gamestate(Board board, Rules rules);
    void create_players(Gamestate* game_state);

    Gamestate setup_game(std::string rule_dir, std::string board_dir, std::string rand_dir) {

        Rules rules = load_rules(rule_dir);

        Board board = load_board(board_dir, rand_dir);

        Gamestate game_state = create_gamestate(board, rules);

        create_players(&game_state);

        return game_state;
    }

    bool is_game_over(Gamestate *game_state) {

        int num_players = 0;

        for (Player p: game_state->players) {
            if (!p.destroyed) {
                num_players += 1;
            }
        }

        if (num_players <= game_state->rules.num_players_to_end_game) {
            return true;
        }

        if (game_state->rules.turn_limit != -1) {
            if (game_state->rules.turn_limit <= game_state->num_moves) {
                return true;
            }
        }

        return false;
    }

    void declare_winners(Gamestate *game_state) {
        std::cout << "The winners are" << std::endl;
        for (Player p: game_state->players) {
            if (p.destroyed) {
                continue;
            }
            std::cout << p.name << std::endl;
        }
    }

    Gamestate create_gamestate(Board board, Rules rules) {
        Gamestate game_state = Gamestate();
        game_state.player_turn_id = 0;
        game_state.player_turn_made_move = false;
        game_state.board = board;
        game_state.rules = rules;
        game_state.random_file_index = 0;
        game_state.num_moves = 0;
        return game_state;
    }

    void create_players(Gamestate* game_state) {
        std::vector<std::string> player_names;
        int number_of_players;

        get_number_of_players_and_player_names(&player_names, &number_of_players);

        (*game_state).num_players = number_of_players;
        (*game_state).players = create_players((*game_state).num_players, (*game_state).rules);

        for (int i = 0; i < number_of_players; i++) {
            (*game_state).players.at(i).name = player_names[i];
        }
        return;
    }
}