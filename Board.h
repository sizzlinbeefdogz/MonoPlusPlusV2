#ifndef CPLUSHOARDING_BOARD_H
#define CPLUSHOARDING_BOARD_H

#include <iostream>
#include <string>
#include <vector>
#include "Go.h"
#include "Property.h"

namespace monopoly {
    class Gamestate;
    class Board {
    public:
        Go go;
        std::vector<Property> properties;
        std::vector<int> random_file;
    };
    Board load_board(std::string board_dir, std::string rand_dir);
    void display_board(Gamestate *game_state);
}

#endif //CPLUSHOARDING_BOARD_H
