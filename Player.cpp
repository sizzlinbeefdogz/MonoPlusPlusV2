#include "Player.h"
#include "Gamestate.h"
#include <iostream>
#include "Upgrade.h"
#include <cmath>

namespace monopoly {

    void get_number_of_players_and_player_names(std::vector<std::string>* player_names, int* number_of_players) {

        std::cout << "Enter how many players will be playing: ";

        int num_players;

        std::cin >> num_players;

        std::vector<std::string> p_names;

        for (int i = 1; i <= num_players; i++) {
            std::cout << "Enter the name of player " << i << ": ";
            std::string player_name;
            std::cin >> player_name;
            p_names.push_back(player_name);
        }

        *player_names = p_names;
        *number_of_players = num_players;

        return;
    }

    Player create_player(int player_id, Rules rules) {
        Player p;
        p.player_id = player_id;
        p.cash = rules.starting_cash;
        p.board_position = 0;
        p.destroyed = false;
        return p;
    }

    std::vector<Player> create_players(int num_players, Rules rules) {
        std::vector<Player> players;

        for (int i = 0; i < num_players; i++) {
            Player p = create_player(i, rules);
            players.push_back(p);
        }

        return players;
    }

    void next_player_turn(Gamestate **game_state) {

        // Important!
        (*game_state)->player_turn_made_move = false;

        // Re rolling logic
        if ((**game_state).should_re_roll == true) {
            return; // don't do anything
        }else{
            (**game_state).num_current_re_rolls = 0;
        }

        Player old_player = (*game_state)->players.at((*game_state)->player_turn_id);

        // Handle if next player's turn is above current player
        int index = 0;
        for (Player p:(*game_state)->players) {
            if (p.destroyed == true) {
                continue;
            }

            if (p.player_id > old_player.player_id) {
                (*game_state)->player_turn_id = p.player_id;
                return;
            }

            index += 1;
        }

        // Handle if next players turn is behind current player
        // We restart searching the array and want the next available player
        for (Player p:(*game_state)->players) {
            if (p.destroyed == true) { continue; }

            (*game_state)->player_turn_id = p.player_id;
            break;
        }

        return;
    }

    void player_passed_go(Gamestate ***game_state) {
        int p_id = (**game_state)->player_turn_id;

        int raw_go_cash = ((**game_state)->board.go.amount_of_cash_gained);

        (**game_state)->players.at(p_id).cash += raw_go_cash;

        return;
    }

    void player_passed_go_mult(Gamestate ***game_state) {
        int p_id = (**game_state)->player_turn_id;

        int raw_go_cash = ((**game_state)->board.go.amount_of_cash_gained);
        int go_mult = ((**game_state)->rules.salary_mult);

        int cash = raw_go_cash * go_mult;

        (**game_state)->players.at(p_id).cash += cash;

        return;
    }

    void destroy_player(Gamestate **game_state) {
        int player_to_leave = (*game_state)->player_turn_id;
        (*game_state)->players.at(player_to_leave).destroyed = true;
    }

    bool does_player_needs_to_pay_rent(Gamestate **game_state, Property landed_on, int player_id) {
        Player owner;
        owner.player_id = 99997; // sudo id
        get_owner_of_property((**game_state), landed_on, &owner);

        if (owner.player_id != 99997) { // there a owner

            // property is owned by a owner

            if (owner.player_id != player_id) {

                // deduct rent if the owner is not itself
                int rent = landed_on.rent;

                // check if set is owned buy owner
                bool all_sets = false;
                check_if_owner_owns_whole_set((**game_state), landed_on, &owner, &all_sets);
                if (all_sets) {
                    rent = (landed_on.rent * (**game_state).rules.property_set_mult);
                }

                if (landed_on.num_houses > 0) {
                    int house_mult = pow(2, (landed_on.num_houses -1));
                    rent = rent * house_mult;
                }

                // Give player an opportunity to sell properties
                int new_cash = (*game_state)->players.at(player_id).cash - rent;

                while(new_cash < 0) {
                    Upgrade upgrader;

                    std::vector<Property> properties = upgrader.get_sellable_upgrades(game_state);

                    if (properties.size() > 0) {
                        // Try sell properties first
                        cout << "You have $" << (*game_state)->players.at(player_id).cash << " but you owe $" << rent << std::endl;
                        cout << "Pick an upgrade to sell to makeup the difference" << std::endl;
                        upgrader.display_sellable_upgrades(properties, false);
                        upgrader.sell_upgrade(game_state, properties);
                    }else{
                        break;
                    }
                }
                new_cash = (*game_state)->players.at(player_id).cash - rent;

                if (new_cash < 0) {
                    // Pay cash & go bankrupt
                    (*game_state)->players.at(player_id).cash -= rent;
                    (*game_state)->players.at(owner.player_id).cash += rent;
                    transfer_property_and_cash_from_player_to_player(&game_state, player_id, owner.player_id);
                    std::cout << (**game_state).players.at(player_id).name << " went bankrupt to " << owner.name << " for landing on " << landed_on.name;
                    std::cout << std::endl;
                } else {
                    // Pay cash & not go bankrupt
                    (*game_state)->players.at(player_id).cash -= rent;
                    (*game_state)->players.at(owner.player_id).cash += rent;
                    std::cout << (**game_state).players.at(player_id).name << " paid " << owner.name << " $" << rent;
                    std::cout << " for landing on " << landed_on.name;
                    std::cout << std::endl;
                }
            } else {
                // player owns property
                std::cout << (*game_state)->players.at(player_id).name << ", you landed on " << landed_on.name << " which you already own" << std::endl;
            }

            return true;
        }

        return false;
    }

    void transfer_property_and_cash_from_player_to_player(Gamestate ***game_state, int payer_id, int receiver_id) {
        // Transfer cash
        (**game_state)->players.at(receiver_id).cash += (**game_state)->players.at(payer_id).cash;

        // Transfer property
        for (Property p: (**game_state)->players.at(payer_id).owned_properties) {
            (**game_state)->players.at(receiver_id).owned_properties.push_back(p);
        }
    }

    bool can_player_afford_to_buy_property(Player player, Property property) {

        if (player.cash >= property.property_cost) {
            return true;
        }

        std::cout << player.name;
        std::cout << ", you don't have enough money to afford ";
        std::cout << property.name;
        std::cout << ".";
        std::cout << std::endl;

        std::cout << property.name << " costs $" << property.property_cost << " but you only have $" << player.cash;
        std::cout << std::endl;

        return false;
    }

    int get_player_to_inspect(Gamestate game_state) {
        std::cout << "Which player would you like to inspect?" << std::endl;

        for (Player p: game_state.players) {
            if (p.destroyed) {
                continue;
            }
            std::cout << p.player_id << " for Player " << p.player_id << std::endl;
        }

        std::cout << "Your choice: ";

        int player_id;
        std::cin >> player_id;
        return player_id;
    }
}