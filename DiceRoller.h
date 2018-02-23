#ifndef CPLUSHOARDING_DICEROLLER_H
#define CPLUSHOARDING_DICEROLLER_H

#include "Gamestate.h"
#include <vector>

using std::cout;
using std::endl;

namespace monopoly {
    class DiceRoller {
    private:
        std::vector<int>get_dice(Gamestate& game_state) {
            unsigned int index = (game_state).random_file_index;

            // This stops the index from overflowing
            if (index == (game_state).board.random_file.size()-1) {
                (game_state).random_file_index = 0;
                index = 0;
            }

            int diceOne = 0;
            int diceTwo = 0;

            diceOne = (game_state).board.random_file.at(index);
            (game_state).random_file_index++;

            diceTwo = (game_state).board.random_file.at(index + 1);
            (game_state).random_file_index++;

            diceOne = (diceOne) + 1;
            diceTwo = (diceTwo) + 1;

            if (diceOne > 10) {
                diceOne = (diceOne % 6); // +1?
                diceTwo = (diceTwo % 6); // +1?
            }

            std::vector<int> dice;
            dice.push_back(diceOne);
            dice.push_back(diceTwo);
            return dice;
        }
    public:
        int roll_dice_and_get_moves(Gamestate ***game_state) {

            (***game_state).should_re_roll = false;

            std::vector<int> dice = get_dice(***game_state);
            int diceOne = dice[0];
            int diceTwo = dice[1];

            int number_of_re_rolls = (***game_state).rules.max_rerolls;

            if (diceOne == diceTwo) {
                (***game_state).num_current_re_rolls += 1;

                if ((***game_state).num_current_re_rolls < (number_of_re_rolls+1)) {
                    (***game_state).should_re_roll = true;
                }
            }

            return diceOne + diceTwo;
        }
    };
}

#endif //CPLUSHOARDING_DICEROLLER_H
