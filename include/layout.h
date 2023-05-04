#include "olcPixelGameEngine.h" //just for vf2d
#include "state.h"
#include "state_manager.h"

class Layout {
    std::unordered_map<StateID,olc::vf2d> state_pos;
public:
    Layout(StateManager& sm, float seperator, int max_x, int max_y)
    {
        float cur_x = seperator;        
        float cur_y = seperator;        

        for(const auto &[id, base] : sm.states){
            state_pos.insert({id, {cur_x, cur_y}});
            cur_x += seperator;
            if (cur_x > max_x){
                cur_x = 0;
                cur_y += seperator;
            }
        }
    }

    olc::vf2d pos(StateID id){
        return state_pos.at(id);
    }
};