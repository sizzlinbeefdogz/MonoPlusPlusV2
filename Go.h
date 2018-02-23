#ifndef CPLUSHOARDING_GO_H
#define CPLUSHOARDING_GO_H

#include "Space.h"
#include "Property.h"
#include <string>

namespace monopoly {
    class Go : public Property {
    public:
        int amount_of_cash_gained;
    };
    Go load_go_from_rule(std::string rule);
}
#endif //CPLUSHOARDING_GO_H
