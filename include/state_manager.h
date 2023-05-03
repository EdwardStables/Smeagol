#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include <set>
#include <assert.h>

#include "state.h"
#include "inputs.h"

class StateManager {
    StateID next_state_id = 0;
    InputID next_input_id = 0;
    std::unordered_map<StateID,StateBase> states;
    std::unordered_map<InputID,InputBase> inputs;
    std::unordered_map<StateID,Transition> transitions;

public:
    StateID add_state(std::string name);
    InputID add_input(std::string name);
    void connect(StateID initial, StateID result, InputID input);
    std::string summary_string();
};