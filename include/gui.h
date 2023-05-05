#pragma once

#include <optional>

#include "olcPixelGameEngine.h"
#include "state_manager.h"
#include "state.h"
#include "inputs.h"

struct GUIState {
    olc::vf2d pos;
    float radius;
    StateBase state;
    enum {NONE, HOVER, SELECT} draw_state = NONE;

    void draw(olc::PixelGameEngine& pge);    
    bool intersects(olc::vf2d test_pos);

    GUIState(olc::vf2d pos, float radius, StateBase state) :
        pos(pos),
        radius(radius),
        state(state)
    {}
};

class StateCanvas {
    olc::PixelGameEngine& pge;
    StateManager& sm;
    std::unordered_map<StateID, GUIState>& states;
    const olc::vi2d _pos;
    const olc::vi2d _size;
public:
    StateCanvas(
        olc::PixelGameEngine& pge,
        StateManager& sm,
        olc::vi2d pos,
        olc::vi2d size,
        std::unordered_map<StateID, GUIState>& states
    ) : pge(pge), sm(sm), _pos(pos), _size(size), states(states)
    {}
    void update();
    void draw();
    olc::vf2d size() {return _size;}
    olc::vf2d pos() {return _pos;}
    std::optional<olc::vf2d> screen_to_canvas(olc::vf2d screen_pos);
};

class SmeagolGUI : public olc::PixelGameEngine {
    StateManager& sm;
    StateCanvas sc;
    std::unordered_map<StateID, GUIState> states;

public:
    SmeagolGUI(StateManager& sm)
    : sm(sm),
      sc(StateCanvas(*this, sm, {100,0}, {699, 399}, states))
    {
        sAppName = "Smeagol";
    }
public:
    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;
};

void run_gui(StateManager& sm);