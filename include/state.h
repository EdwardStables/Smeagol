#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <set>

#include "inputs.h"

using StateID = uint32_t;
using Transition = std::unordered_map<InputID,std::set<StateID>>;

class StateBase {
public:
    const std::string name;
    const StateID id;
    const Transition& transition;

    StateBase(std::string name, StateID id, const Transition& transition) :
        name(name),
        id(id),
        transition(transition)
    {}
};