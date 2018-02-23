#include "Board.h"
#include "Gamestate.h"
#include <fstream>
#include <iomanip>
#include <string>


using std::cout;
using std::endl;

namespace monopoly {

    void print_string_name_for_property_on_board(Board board, int i);
    std::vector<int> parse_random_file(std::string rand_dir);
    int max_character_in_name(Board board);
    void print_players_at_board_position(Gamestate* game_state, int i);
    void print_owner_at_board_position(Gamestate** game_state, int i);
    void print_upgrades_at_board_position(Gamestate** game_state, int i);

    std::vector<int> parse_random_file(std::string rand_dir) {
        std::vector<int> arr;

        // Open random file
        std::ifstream inFile;
        inFile.open(rand_dir);

        // Read each line from file into rule struct
        std::string line_to_read;
        while (getline(inFile, line_to_read)) {

            if (line_to_read[0] == ' ' || line_to_read.empty()) {
                continue;
            }

            arr.push_back(atoi(line_to_read.c_str()));
            //cout << endl << atoi(line_to_read.c_str()) << endl;
        }

        // Close rule file
        inFile.close();

        return arr;
    }

    Board load_board(std::string board_dir, std::string rand_dir) {

        // Create board
        Board board = Board();

        // Load random file
        board.random_file = parse_random_file(rand_dir);

        // Load CSV file
        std::ifstream inFile;
        inFile.open(board_dir);

        std::string line_to_read;
        int counter = 0;

        while (getline(inFile, line_to_read)) {
            counter += 1;

            // Skip first few unimportant lines
            if (counter < 5) { continue; }

            line_to_read = line_to_read.substr(line_to_read.find(",") + 1); // Get line to read

            if (counter == 5) {
                board.go = load_go_from_rule(line_to_read); // Load Go
            } else {
                board.properties.push_back(load_property_from_rule(line_to_read)); // Load property
            }
        }

        // Close file
        inFile.close();

        return board;
    }

    void display_board(Gamestate *game_state) {

        int x = game_state->board.properties.size() + 1; // +1 for go file

        std::cout << "Space Number | Space Name | Owner | Upgrades | Players " << std::endl;

        for (int i = 0; i < x; i++) {
            // Print board space number & space name
            print_string_name_for_property_on_board(game_state->board, i);

            // Print owners
            print_owner_at_board_position(&game_state, i);

            print_upgrades_at_board_position(&game_state, i);

            // Print players e.g [Jeff : $300]
            print_players_at_board_position(game_state, i);

            // End with new line
            std::cout << std::endl;
        }
    }

    void print_upgrades_at_board_position(Gamestate** game_state, int i) {

        int num_houses = 0;
        int num_hotels = 0;

        if (i == 0) {
            num_houses = (**game_state).board.go.num_houses;
        }else{
            num_houses = (**game_state).board.properties.at(i-1).num_houses;
            num_hotels = (**game_state).board.properties.at(i-1).num_hotels;
        }

        for (int i = 0; i < num_houses; i++) {
            std::cout << "h";
        }

        for (int i = 0; i < num_hotels; i++) {
            std::cout << "H";
        }

        std::cout << " | ";
        return;
    }

    void print_owner_at_board_position(Gamestate** game_state, int i) {
        if (i == 0) {
            std::cout << " None | ";
            return;
        }

        Property property_to_check = (*game_state)->board.properties.at(i-1); // minus 1 because of go file

        for (Player player : (*game_state)->players) {
            for (Property player_property : player.owned_properties) {
                if (player_property.name == property_to_check.name && player.destroyed == false) {
                    std::cout << " " << player.name << " | ";
                    return;
                }
            }
        }

        std::cout << " None | ";
        return;
    }

    void print_players_at_board_position(Gamestate* game_state, int i) {
        // Print any players at position
        std::vector<int> player_pos;
        for (Player p : game_state->players) {
            if (p.destroyed) { continue; }

            if (p.board_position == i) {
                bool first = true;

                for (int pos : player_pos) {
                    if (pos == i) { first = false; }
                }

                if (first == true) { std::cout << " "; } else { std::cout << ", "; }

                std::cout << "[" << p.name << " : " << "$" << p.cash << "]";
                player_pos.push_back(i);
            }
        }
    }

    void print_string_name_for_property_on_board(Board board, int i) {
        int max_char = max_character_in_name(board);

        // Display name
        int current_name_length = 0;

        std::cout << i;
        std::cout << " | ";

        if (i == 0) {
            std::cout << board.go.name;
            current_name_length = board.go.name.length();
        } else {
            std::cout << board.properties.at(i - 1).name;
            current_name_length = board.properties.at(i - 1).name.length();
        }

        std::cout << std::setw(max_char - current_name_length + 2) << std::right << " |";

        return;
    }

    int max_character_in_name(Board board) {
        int char_length = 0;

        int x = board.properties.size() + 1; // +1 for go file
        for (int i = 0; i < x; i++) {
            int temp_length = 0;

            if (i == 0) {
                temp_length = board.go.name.length();
            } else {
                temp_length = board.properties.at(i - 1).name.length();
            }

            if (temp_length > char_length) {
                char_length = temp_length;
            }
        }

        return char_length;
    }
}