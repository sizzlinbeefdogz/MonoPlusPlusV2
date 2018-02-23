//
// Created by Josh Arnold on 2/20/18.
//

#ifndef CPLUSHOARDING_UPGRADE_H
#define CPLUSHOARDING_UPGRADE_H

#include <iostream>
#include "Property.h"
#include "Gamestate.h"
#include <vector>
#include <algorithm>

using std::cout;
using std::endl;
using std::sort;

namespace monopoly {
    struct sorting_by_name_key
    {
        inline bool operator() (const Property& struct1, const Property& struct2)
        {
            if (struct1.set_id == struct2.set_id) {
                return (struct1.intraset_id < struct2.intraset_id);
            }
            return (struct1.set_id < struct2.set_id);
        }
    };

    class Upgrade {
    public:
        void must_build_evenly(Gamestate game_state, std::vector<Property>& upgradable_properties, bool upgrade=true) {

            // New array
            std::vector<Property> filtered_properties;

            // Split into sets
            std::vector<std::vector<Property>> sets(upgradable_properties.size());
            for (Property& p : upgradable_properties) {
                sets[p.set_id].push_back(p);
            }

            // Loop each set
            for (std::vector<Property> set : sets) {

                // Skip empty sets
                if (set.size() == 0) { continue; }

                // Loop properties in set
                Property lowest = set[0];
                std::vector<Property> lowest_properties;

                for (Property p : set) {

                    if (upgrade) {
                        if (lowest.upgrade_count() > p.upgrade_count()) {
                            lowest = p;
                            lowest_properties.clear();
                            lowest_properties.push_back(p);

                        }else if (lowest.upgrade_count() == p.upgrade_count()) {
                            lowest_properties.push_back(p);
                        }
                    }else{
                        if (lowest.upgrade_count() < p.upgrade_count()) {
                            lowest = p;
                            lowest_properties.clear();
                            lowest_properties.push_back(p);

                        }else if (lowest.upgrade_count() == p.upgrade_count()) {
                            lowest_properties.push_back(p);
                        }
                    }
                }

                for (Property low : lowest_properties) {
                    filtered_properties.push_back(low);
                }
            }

            // Update
            upgradable_properties = filtered_properties;

            return;
        }

        bool can_player_upgrade(Gamestate game_state, std::vector<Property>& properties) {

            std::vector<Property> upgradable_properties;

            Player current_player = game_state.current_player();

            // Check user owns all properties in the set & enough cash
            for (Property property : current_player.owned_properties) {
                bool player_owns_all_sets = false;

                check_if_owner_owns_whole_set(game_state, property, &current_player, &player_owns_all_sets);

                if ((player_owns_all_sets == true) && (current_player.cash > property.upgrade_cost()) && (property.num_hotels != 1)) {
                    upgradable_properties.push_back(property);
                }
            }

            // Check “Must Build Evenly” rule
            if (game_state.rules.must_build_houses_evenly) {
                must_build_evenly(game_state, upgradable_properties);
            }

            // Sort these bad boys
            std::sort(upgradable_properties.begin(), upgradable_properties.end(), sorting_by_name_key());

            properties = upgradable_properties;

            if (properties.size() > 0) {
                return true;
            }

            return false;
        }

        void display_upgradable_properties(std::vector<Property>& properties) {

            cout << "Which property do you want to upgrade?" << endl;

            int i = 0;
            for (Property p : properties) {
                cout << i << ". " << p.name << " [$" << p.upgrade_cost() << "]" << endl;
                i++;
            }

            cout << "Your choice: ";

            return;
        }

        void display_sellable_upgrades(std::vector<Property>& properties, bool display_text=true) {

            if (display_text) {
                cout << "Which property do you want to downgrade?" << endl;
            }

            int i = 0;
            for (Property p : properties) {
                cout << i << ". " << p.name << " [$" << p.down_grade_cash_recovered() << "]" << endl;
                i++;
            }

            cout << "Your choice: ";

            return;
        }

        void purchase_upgrade(Gamestate **game_state, std::vector<Property>& properties) {
            int property_index = 0;
            std::cin >> property_index;

            Property p_to_buy = properties.at(property_index);

            (**game_state).players.at((**game_state).player_turn_id).cash -= p_to_buy.upgrade_cost();

            for (Property& p : (**game_state).board.properties) {
                if (p.name == properties.at(property_index).name) {
                    p.upgrade();
                    break;
                }
            }

            transfer_properties_from_gamestate_to_owners(&game_state);

            return;
        }

        void sell_upgrade(Gamestate **game_state, std::vector<Property>& properties) {
            int property_index = 0;
            std::cin >> property_index;

            Property p_to_buy = properties.at(property_index);

            (**game_state).players.at((**game_state).player_turn_id).cash += p_to_buy.down_grade_cash_recovered();

            for (Property& p : (**game_state).board.properties) {
                if (p.name == properties.at(property_index).name) {
                    p.down_grade();
                }
            }

            transfer_properties_from_gamestate_to_owners(&game_state);

            return;
        }

        void downgrade_player_properties(Gamestate **game_state) {
            for (Property& p : (**game_state).players.at((**game_state).player_turn_id).owned_properties) {
                p.num_hotels = 0;
                p.num_houses = 0;

                // Updater
                for (Property& board_property : (**game_state).board.properties) {
                    if (board_property.name == p.name) {
                        board_property = p;
                    }
                }
            }
        }

        std::vector<Property> get_sellable_upgrades(Gamestate** game_state) {
            transfer_properties_from_gamestate_to_owners(&game_state);

            Player current = (**game_state).current_player();

            std::vector<Property> properties;

            for (Property p : current.owned_properties) {
                if ((p.num_houses > 0) || (p.num_hotels > 0)) {
                    properties.push_back(p);
                }
            }

            // Check “Must Build Evenly” rule
            if ((**game_state).rules.must_build_houses_evenly) {
                must_build_evenly(**game_state, properties, false);
            }

            // Sort these bad boys
            std::sort(properties.begin(), properties.end(), sorting_by_name_key());

            return properties;
        }
    };
}

#endif //CPLUSHOARDING_UPGRADE_H
