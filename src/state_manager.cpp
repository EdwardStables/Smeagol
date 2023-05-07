#include "state_manager.h"

StateID StateManager::add_state(std::string name){
    StateID id = next_state_id++;
    Transition t = transitions[id];
    states.insert({id, StateBase(name, id, t)});
    return id;
}

void StateManager::delete_state(StateID id){
    if (!states.count(id)) return;
    states.erase(id);

    if (transitions.count(id)){
        transitions.erase(id);
    }

    for (auto &[first, transition] : transitions){
        for (auto &[input, end_set] : transition){
            if (end_set.count(id)){
                end_set.erase(id);
            }
        }
    }
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
void StateManager::delete_connection(StateID initial, StateID result, InputID input){
    if (!transitions.count(initial)) return;
    if (!transitions[initial].count(input)) return;
    if (!transitions[initial][input].count(result)) return;
    transitions[initial][input].erase(result);
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