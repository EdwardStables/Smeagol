#pragma once

#include "olcPixelGameEngine.h"
#include "state_manager.h"
#include "state.h"
#include "inputs.h"

struct GUIState {
    olc::vf2d pos;
    float radius;
    StateBase state;

    void draw(olc::PixelGameEngine& pge);    

    GUIState(olc::vf2d pos, float radius, StateBase state) :
        pos(pos),
        radius(radius),
        state(state)
    {}
};

class SmeagolGUI : public olc::PixelGameEngine {
    StateManager& sm;

    std::vector<GUIState> states;

public:
    SmeagolGUI(StateManager& sm) : sm(sm) {
        sAppName = "Smeagol";
    }
public:
    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;
};

void run_gui(StateManager& sm);