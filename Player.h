#ifndef CPLUSHOARDING_PLAYER_H
#define CPLUSHOARDING_PLAYER_H

#include <vector>
#include "Rules.h"
#include "Property.h"
#include <string>

namespace monopoly {
    class Gamestate;
    class Player {
    public:
        std::string name;
        int player_id;
        int board_position;
        int cash;
        bool destroyed;
        std::vector<int> properties_owned_per_set;
        std::vector<Property> owned_properties;
        bool lost_bid = false;
        void reset_bidding() {
            this->lost_bid = false;
        }
    };

    void get_number_of_players_and_player_names(std::vector<std::string>* player_names, int* number_of_players);
    std::vector<Player> create_players(int num_players, Rules rules);
    void next_player_turn(Gamestate **game_state);
    void player_passed_go(Gamestate ***game_state);
    void player_passed_go_mult(Gamestate ***game_state);
    void destroy_player(Gamestate **game_state);
    bool does_player_needs_to_pay_rent(Gamestate **game_state, Property landed_on, int player_id);
    void transfer_property_and_cash_from_player_to_player(Gamestate ***game_state, int payer_id, int receiver_id);
    bool can_player_afford_to_buy_property(Player player, Property property);
    int get_player_to_inspect(Gamestate game_state);
}

#endif //CPLUSHOARDING_PLAYER_H
