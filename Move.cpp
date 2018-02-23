#include "Move.h"
#include "DiceRoller.h"
#include "Upgrade.h"
#include "Auction.h"

namespace monopoly {

    void print_action_options(int player_id, Gamestate game_state);

    int get_player_action();

    Property roll_dice_and_move_player(Gamestate **game_state);

    Move get_move(Gamestate *game_state) {

        print_action_options(game_state->player_turn_id, (*game_state));

        int action = get_player_action();

        Move move;
        move.move_ended = false;
        move.action_id = action;

        return move;
    }

    void roll_dice(Gamestate *game_state, Move *move) {

        // temp
        Gamestate **gm = &game_state;
        transfer_properties_from_gamestate_to_owners(&gm);

        // Action 1 roll dice
        // player ID
        int player_id = game_state->player_turn_id;

        // Roll dice & move player)
        Property landed_on = roll_dice_and_move_player(&game_state);

        // Landed on go, let's exit this
        if (landed_on.name == game_state->board.go.name) {

            // Next move
            next_player_turn(&game_state);
            move->move_ended = true;
            game_state->num_moves += 1;
            return;
        }

        // Check if player owes rent or not
        bool rent = does_player_needs_to_pay_rent(&game_state, landed_on, player_id);

        if (!rent) {

            // Check if player can afford property
            bool afford = can_player_afford_to_buy_property(game_state->players.at(player_id), landed_on);
            if (!afford) {
                // Check to start auction if needed
                Auction a;
                a.start_auction(&game_state);
                // Next move
                next_player_turn(&game_state);
                move->move_ended = true;
                game_state->num_moves += 1;
                return;
            }

            // Offer to buy property
            offer_to_buy_property(&game_state, landed_on);

        } else if (game_state->players.at(player_id).cash < 0) {
            // Player went bankrupt
            destroy_player(&game_state);

            // Next move
            next_player_turn(&game_state);
            move->move_ended = true;
            game_state->num_moves += 1;
            return;
        }

        // Next move
        next_player_turn(&game_state);
        move->move_ended = true;
        game_state->num_moves += 1;
        return;
    }
    void leave_game(Gamestate *game_state, Move *move) {
        // Leave game
        Upgrade upgrader;
        upgrader.downgrade_player_properties(&game_state);

        destroy_player(&game_state);
        next_player_turn(&game_state);
        move->move_ended = true;
        game_state->num_moves += 1;
    }

    void buy_upgrade(Gamestate *game_state, Move *move) {
        Upgrade upgrader;

        std::vector<Property> properties;

        bool can_upgrade = upgrader.can_player_upgrade(*game_state, properties);

        if (can_upgrade) {
            upgrader.display_upgradable_properties(properties);
            upgrader.purchase_upgrade(&game_state, properties);
        }else{
            cout << "You don't have any properties that you can upgrade" << std::endl;
        }

    }
    void sell_upgrade(Gamestate *game_state, Move *move) {
        Upgrade upgrader;

        std::vector<Property> properties = upgrader.get_sellable_upgrades(&game_state);

        if (properties.size() > 0) {

            upgrader.display_sellable_upgrades(properties);
            upgrader.sell_upgrade(&game_state, properties);

        }else{
            cout << "You don't have any upgrades that you can sell" << endl;
        }
    }

    void make_move(Gamestate *game_state, Move *move) {
        int action = move->action_id;

        if (action == 1) {
            roll_dice(game_state, move);
        } else if (action == 2) {
            buy_upgrade(game_state, move);
        }else if (action == 3) {
            sell_upgrade(game_state, move);
        }else{
            leave_game(game_state, move);
        }
    }

    void print_action_options(int player_id, Gamestate game_state) {
        std::cout << game_state.players.at(player_id).name << " please enter your move" << std::endl;

        if (game_state.player_turn_made_move == false) {
            std::cout << "1 to roll dice" << std::endl;
        } else {
            std::cout << "1 to end turn" << std::endl;
        }

        std::cout << "2 to upgrade a property with a house or hotel" << std::endl;

        std::cout << "3 to sell a house or hotel" << std::endl;

        std::cout << "4 to leave the game" << std::endl;

        std::cout << "Your move: ";
    }

    int get_player_action() {
        int action;
        std::cin >> action;
        return action;
    }

    Property roll_dice_and_move_player(Gamestate **game_state) {

        int player_id = (*game_state)->player_turn_id;
        int player_pos = (*game_state)->players.at(player_id).board_position;

        DiceRoller roller;
        int num_moves = roller.roll_dice_and_get_moves(&game_state);

        std::cout << (*game_state)->players.at(player_id).name << ", you rolled a " << num_moves << std::endl;

        int new_pos = player_pos + num_moves;

        int num_prop = (*game_state)->board.properties.size() + 1;

        int num_times_pass_go = new_pos / (num_prop);

        for (int i = 0; i < num_times_pass_go; i++) {
            if (i == (num_times_pass_go - 1)) {
                break;// break if it's last one, then decide in move for last to either mul or
            }
            player_passed_go(&game_state);
        }

        while (new_pos >= num_prop) {
            new_pos -= num_prop;
        }

        (*game_state)->players.at(player_id).board_position = new_pos;
        (*game_state)->player_turn_made_move = true;

        if (new_pos == 0) {
            if (num_times_pass_go != 0) {
                player_passed_go_mult(&game_state);
            }
            Property p;
            p.name = (*game_state)->board.go.name;
            return p;
        } else if (num_times_pass_go != 0) {
            player_passed_go(&game_state);
        }

        Property p = (*game_state)->board.properties.at(new_pos - 1);
        return p;
    }
}