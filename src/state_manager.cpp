#include "state_manager.h"

StateID StateManager::add_state(std::string name){
    StateID id = next_state_id++;
    Transition t = transitions[id];
    states.insert({id, StateBase(name, id, t)});
    return id;
}

InputID StateManager::add_input(std::string name){
    InputID id = next_input_id++;
    inputs.insert({id, InputBase(name, id)});
    return id;
}

void StateManager::connect(StateID initial, StateID result, InputID input){
    assert(states.count(initial));
    assert(states.count(result));
    assert(inputs.count(input));
    transitions[initial][input].insert(result);
}

std::string StateManager::summary_string(){
    std::string out;
    for (const auto &[initial, transition] : transitions){
        for (const auto &[input, result_set] : transition){
            for (const auto &result : result_set){
                out += states.at(initial).name + " -- ";
                out += inputs.at(input).name + " --> ";
                out += states.at(result).name + "\n";
            }
        }
    }
    return out;
}