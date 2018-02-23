#include "Property.h"
#include "Rules.h"
#include "Gamestate.h"
#include "Player.h"
#include <string>
#include <algorithm>
#include "Auction.h"

namespace monopoly {

    Property load_property_from_rule(std::string rule) {
        Property prop;
        std::vector<std::string> array_of_strings = parse_comma_string(rule);
        prop.set_id = std::stoi(array_of_strings[0]);
        prop.intraset_id = std::stoi(array_of_strings[1]);
        prop.name = array_of_strings[2];
        prop.property_cost = std::stoi(array_of_strings[3]);
        prop.house_cost = std::stoi(array_of_strings[4]);
        prop.hotel_cost = std::stoi(array_of_strings[5]);
        prop.rent = std::stoi(array_of_strings[6]);
        prop.rent_with_house = std::stoi(array_of_strings[7]);
        prop.rent_with_hotel = std::stoi(array_of_strings[8]);
        prop.original_rent = prop.rent;
        return prop;
    }

    void get_owner_of_property(Gamestate game_state, Property property, Player *owner) {
        for (Player player:game_state.players) {
            for (Property prop:player.owned_properties) {
                if (prop.set_id == property.set_id && prop.intraset_id == property.intraset_id &&
                    prop.name == property.name) {
                    // Property is owned by player
                    if (!player.destroyed) {
                        (*owner) = player;
                    }
                }
            }
        }
    }

    void
    check_if_owner_owns_whole_set(Gamestate game_state, Property property, Player *owner, bool *player_owns_all_sets) {
        int set_id_to_count = property.set_id;

        int num_prop_in_set = 0;
        int num_prop_owned = 0;

        for (Property prop:game_state.board.properties) {
            if (prop.set_id == set_id_to_count) {
                num_prop_in_set += 1;
            }
        }

        for (Property prop: owner->owned_properties) {
            if (prop.set_id == set_id_to_count) {
                num_prop_owned += 1;
            }
        }

        if (num_prop_in_set == num_prop_owned) {
            (*player_owns_all_sets) = true;
        } else {
            (*player_owns_all_sets) = false;
        }
    }

    bool offer_to_buy_property(Gamestate **game_state, Property property) {
        // Deal with buying
        std::cout << "Would you like to buy ";
        std::cout << property.name << " for $" << property.property_cost << "?" << std::endl;
        std::cout << "Rent on " << property.name << " is $" << property.rent;
        std::cout << " ";
        //std::cout << "You currently have $" << (*game_state)->players.at((*game_state)->player_turn_id).cash << std::endl;
        std::cout << "Enter y for yes or n for no: ";
        std::string buy_property;
        std::cin >> buy_property;

        if (tolower(buy_property[0]) == 'y') {
            (*game_state)->players.at((*game_state)->player_turn_id).cash -= property.property_cost;
            (*game_state)->players.at((*game_state)->player_turn_id).owned_properties.push_back(property);

            std::cout << (**game_state).players.at((**game_state).player_turn_id).name << " bought " << property.name << " for $" << property.property_cost;

            return true;
        } else {
            // Check to start auction if needed
            Auction a;
            a.start_auction(game_state);
            return false;
        }
    }

    void print_property_name_set(std::vector<Property> properties, int set_id) {

        // get names
        std::vector<std::string> names;
        for (Property p : properties) {
            if (p.set_id == set_id) {
                names.push_back(p.name);
            }
        }

        // sort names
        std::sort(names.begin(), names.end());

        // display names
        std::string separator = " | ";
        int j = 0;
        for (std::string name : names) {
            if (j > 0) {
                std::cout << separator;
            }
            std::cout << name;
            j += 1;
        }
    }

    void transfer_properties_from_gamestate_to_owners(Gamestate*** game_state) {

        for (Player& player : (***game_state).players) {

            for (Property& property : player.owned_properties) {

                for (Property& board_property : (***game_state).board.properties) {

                    if (board_property.name == property.name) {
                        property = board_property;
                    }
                }
            }
        }
        return;
    }
}