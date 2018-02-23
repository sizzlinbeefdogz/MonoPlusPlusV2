#include "Go.h"

namespace monopoly{

Go load_go_from_rule(std::string rule) {

    // Starting cash
    std::string go_cash = rule.substr(0, rule.find(","));

    // Go name
    std::string name = rule;
    name.erase(0, rule.find(",") + 1);
    name = name.substr(0, name.find(",,,,,,,"));

    // Chuck it all in a class
    Go go = Go();
    go.amount_of_cash_gained = stoi(go_cash);
    go.name = name;

    return go;
}

}