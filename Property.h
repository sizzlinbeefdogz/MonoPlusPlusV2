#ifndef CPLUSHOARDING_PROPERTY_H
#define CPLUSHOARDING_PROPERTY_H

#include "Space.h"
#include <vector>
#include <string>

namespace monopoly {
    class Gamestate;
    class Player;
    class Property : public Space {
    public:
        int num_houses = 0;
        int num_hotels = 0;
        int property_cost;
        int hotel_cost;
        int house_cost;
        int rent;
        int rent_with_house;
        int rent_with_hotel;
        int original_rent;

        int upgrade_count() {
            if (num_hotels == 1) {
                return 5;
            }else{
                return num_houses;
            }
        }

        int upgrade_cost() {
            if (num_houses == 4) {
                return hotel_cost;
            }
            return this->house_cost;
        }

        void upgrade() {
            if (num_houses == 4) {
                num_houses = 0;
                num_hotels = 1;
                rent = rent_with_hotel; // update rent
            }else{
                num_houses += 1;
                rent = rent_with_house; // update rent
            }
            return;
        }

        int down_grade_cash_recovered() {
            if (num_hotels > 0) {
                return hotel_cost/2;
            }else{
                return house_cost/2;
            }
        }

        void down_grade() {
            if (num_hotels > 0) {
                this->num_hotels = 0;
                this->num_houses = 4;
                rent = rent_with_house;
            }else{
                this->num_houses -= 1;
                if (num_houses == 0) {
                    rent = original_rent;
                }
            }
        }
    };
    Property load_property_from_rule(std::string rule);
    void get_owner_of_property(Gamestate game_state, Property property, Player *owner);
    void check_if_owner_owns_whole_set(Gamestate game_state, Property property, Player *owner, bool *player_owns_all_sets);
    bool offer_to_buy_property(Gamestate **game_state, Property property);
    void print_property_name_set(std::vector<Property> properties, int set_id);

    void transfer_properties_from_gamestate_to_owners(Gamestate*** game_state);
}

#endif //CPLUSHOARDING_PROPERTY_H
