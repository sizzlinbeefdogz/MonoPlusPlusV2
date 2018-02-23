#ifndef CPLUSHOARDING_RULES_H
#define CPLUSHOARDING_RULES_H

#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <ctype.h>

using std::cout;

namespace monopoly {

    class Rules {
    public:
        int starting_cash;
        int turn_limit;
        int num_players_to_end_game;
        int property_set_mult;
        int num_houses_before_hotel;
        bool must_build_houses_evenly;
        bool put_money_in_free_parking;
        bool auction_properties;
        int salary_mult;
        int max_rerolls;

    private:
        int ExtractRuleInt(std::string rule) {
            std::string value = rule.substr(rule.find(": ") + 1);
            return stoi(value);
        }
        bool ExtractRuleBool(std::string rule) {
            std::string value = rule.substr(rule.find(": ") + 1);

            char i = tolower(value[1]); // this f***er has to be 1.

            if (i == 'y') {
                return true;
            }else{
                return false;
            }
        }

    public:
        void set_property(std::string line_to_read, int index) {
            switch(index) {
                case 0:
                    starting_cash = ExtractRuleInt(line_to_read);
                    break;
                case 1:
                    turn_limit = ExtractRuleInt(line_to_read);
                    break;
                case 2:
                    num_players_to_end_game = ExtractRuleInt(line_to_read);
                    break;
                case 3:
                    property_set_mult = ExtractRuleInt(line_to_read);
                    break;
                case 4:
                    num_houses_before_hotel = ExtractRuleInt(line_to_read);
                    break;
                case 5:
                    must_build_houses_evenly = ExtractRuleBool(line_to_read);
                    break;
                case 6:
                    put_money_in_free_parking = ExtractRuleBool(line_to_read);
                    break;
                case 7:
                    auction_properties = ExtractRuleBool(line_to_read);
                    break;
                case 8:
                    salary_mult = ExtractRuleInt(line_to_read);
                    break;
                case 9:
                    max_rerolls = ExtractRuleInt(line_to_read);
                    break;
            }
        }
    };

    Rules load_rules(std::string rule_dir);
    std::vector<std::string> parse_comma_string(std::string string_to_parse);
}

#endif //CPLUSHOARDING_RULES_H
