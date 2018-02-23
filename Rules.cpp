#include "Rules.h"
#include <iostream>
#include <fstream>
#include <sstream>
//#include <curses.h>

namespace monopoly {

    Rules load_rules(std::string rule_dir) {

        // Open rule file
        std::ifstream inFile;
        inFile.open(rule_dir);

        // Check rule file exists otherwise take matters into our own hands...
        if (!inFile) {
            std::cerr << "Unable to open rule file :(";
            exit(1);
        }

        // Define our instance
        Rules rules = Rules();

        // Read each line from file into rule class
        std::string line_to_read;
        int counter = 0;
        while (getline(inFile, line_to_read)) {
            rules.set_property(line_to_read, counter);
            counter += 1;
        }

        // Close rule file
        inFile.close();

        return rules;
    }

    std::vector<std::string> parse_comma_string(std::string string_to_parse) {

        std::stringstream ss(string_to_parse);

        std::vector<std::string> result;

        while (ss.good()) {
            std::string sub_string;
            getline(ss, sub_string, ',');
            result.push_back(sub_string);
        }

        return result;
    }
}