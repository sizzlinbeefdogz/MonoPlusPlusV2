//
// Created by Josh Arnold on 2/21/18.
//

#ifndef CPLUSHOARDING_AUCTION_H
#define CPLUSHOARDING_AUCTION_H

#include "Gamestate.h"
#include <string>

using std::cout;
using std::endl;

namespace monopoly {
    class Auction {
    private:
        Player get_first_bidding_player(Gamestate game_state) {
            for (Player p : (game_state).players) {
                if (!p.destroyed) {
                    return p;
                }
            }

            Player p;
            return p;
        }

        void print_bid_prompt(bool no_current_bid, Property p, Player current_player, int current_bid,
                              Player bidding_player) {
            if (no_current_bid) {
                cout << "No one has bid on " << p.name << "[$" << p.property_cost << "] yet";
            } else {
                cout << "The current bid for " << p.name << "[$" << p.property_cost << "] is $" << current_bid << " by "
                     << current_player.name;
            }

            cout << endl;
            cout << bidding_player.name << ", enter a bid of at least $" << current_bid + 1
                 << " to bid on the property or a value less than that to leave the auction";
            cout << endl;
            cout << "Your bid: ";
        }

        std::vector<Player> get_bidding_players(Gamestate game_state) {
            std::vector<Player> players;
            for (Player p : (game_state).players) {
                if (!p.destroyed) {
                    p.reset_bidding();
                    players.push_back(p);
                }
            }
            return players;
        }

        void bidder_leave_game(Player bidding_player, std::vector<Player> &players) {
            bidding_player.lost_bid = true;
            for (Player &player : players) {
                if (player.name == bidding_player.name) {
                    player.lost_bid = true;
                    break;
                }
            }
        }

        Player get_next_bid_player(Player bidding_player, std::vector<Player> &players) {

            int i = 0;

            for (Player p : players) {
                if (i == 1) {
                    if (p.lost_bid == false) {
                        return p;
                    }
                }

                if (p.name == bidding_player.name) {
                    i = 1;
                }
            }

            for (Player p : players) {
                if (p.lost_bid == false) {
                    return p;
                }
            }

            return bidding_player;
        }

        bool should_end_auction(std::vector<Player> &players) {

            int size_of = 0;

            for (Player p: players) {
                if (p.lost_bid == false) {
                    size_of++;
                }
            }

            if (size_of == 1) {
                return true;
            }

            return false;
        }

    public:
        void start_auction(Gamestate **game_state) {

            bool start = (**game_state).rules.auction_properties;

            if (start == true) {
                int board_pos = (**game_state).current_player().board_position;
                if (board_pos == 0) { return; }

                Property p = (**game_state).board.properties.at(board_pos - 1);
                cout << "Starting the auction for " << p.name;

                // Current bid
                int current_bid = 0;
                bool no_current_bid = true;
                Player current_player;

                // Players that will bid
                std::vector<Player> players = get_bidding_players(**game_state);

                // Bidding player
                Player bidding_player = get_first_bidding_player(**game_state);

                while (1) {
                    print_bid_prompt(no_current_bid, p, current_player, current_bid, bidding_player);

                    int bid;
                    std::cin >> bid;

                    // Leave game or make a bid
                    if (bid <= current_bid) {
                        bidder_leave_game(bidding_player, players);
                    } else {
                        no_current_bid = false;
                        current_player = bidding_player;
                        current_bid = bid;
                    }

                    // Get the next bidding player
                    bidding_player = get_next_bid_player(bidding_player, players);

                    // Check to see to end auction
                    int size_of = 0;

                    for (Player p: players) {
                        if (p.lost_bid == false) {
                            size_of++;
                        }
                    }

                    if (size_of == 1) {
                        break;
                    }
                }

                // Analyse winner
                if (current_bid > 0) {
                    // Finally we have our winner:
                    current_player.cash -= current_bid;
                    current_player.owned_properties.push_back(p);
                    for (Player &player : (**game_state).players) {
                        if (player.name == current_player.name) {
                            player = current_player;
                        }
                    }

                    cout << current_player.name << " won " << p.name << " for $" << current_bid;
                }else{
                    print_bid_prompt(no_current_bid, p, current_player, current_bid, bidding_player);
                    cout << "No one decided to purchase " << p.name;
                }
            }
        }
    };
}

#endif //CPLUSHOARDING_AUCTION_H
