#pragma once

#include <optional>

#include "olcPixelGameEngine.h"
#include "state_manager.h"
#include "state.h"
#include "inputs.h"

enum e_mode {
    NORMAL, DELETE_STATE
};

struct GUIState {
    olc::vf2d pos;
    float radius;
    StateBase state;
    olc::vf2d grab_offset;
    enum {NONE, DELETE_HOVER, HOVER, SELECT} draw_state = NONE;

    void draw(olc::PixelGameEngine& pge);    
    bool intersects(olc::vf2d test_pos);

    GUIState(olc::vf2d pos, float radius, StateBase state) :
        pos(pos),
        grab_offset(pos),
        radius(radius),
        state(state)
    {}
};

struct Button {
    enum e_button_type {TOGGLE, MOMENTRY};

    std::string name;
    olc::PixelGameEngine& pge;
    const olc::vi2d _pos;
    const olc::vi2d _size;
    e_button_type type;
    enum {NONE, HOVER, SELECT} draw_state = NONE;
    bool button_pressed = false;

    Button(
        olc::PixelGameEngine& pge,
        olc::vi2d pos,
        olc::vi2d size,
        std::string name,
        e_button_type type
    ) : name(name), pge(pge), _pos(pos), _size(size), type(type) {}

    void update();
    void draw() const;
    bool register_update();
};

class ButtonPanel {
    olc::PixelGameEngine& pge;
    e_mode& mode;
    StateManager& sm;
    const olc::vi2d _pos;
    const olc::vi2d _size;
    std::vector<Button> buttons;
    const olc::vi2d margin = {5,5};
public:
    ButtonPanel (
        olc::PixelGameEngine& pge,
        e_mode& mode,
        StateManager& sm,
        olc::vi2d pos,
        olc::vi2d size
    ) : pge(pge), mode(mode), sm(sm), _pos(pos), _size(size)
    {
        std::vector<std::pair<std::string,Button::e_button_type>> button_contents = {
            {"Add State", Button::MOMENTRY},
            {"Remove Mode", Button::TOGGLE}
        };
        olc::vi2d offs = {0, 12 + margin.y};
        int ind = 0;
        for (const auto &[name, type] : button_contents){
            buttons.push_back(Button(pge, _pos + margin + (ind*offs), {_size.x - 2*margin.x, 12}, name, type));
            ind++;
        }

    }
    void update();
    void draw() const;
};

class StateCanvas {
    olc::PixelGameEngine& pge;
    e_mode& mode;
    StateManager& sm;
    std::unordered_map<StateID, GUIState>& states;
    const olc::vi2d _pos;
    const olc::vi2d _size;
    std::optional<StateID> held_id = std::nullopt;
public:
    StateCanvas(
        olc::PixelGameEngine& pge,
        e_mode& mode,
        StateManager& sm,
        olc::vi2d pos,
        olc::vi2d size,
        std::unordered_map<StateID, GUIState>& states
    ) : pge(pge), mode(mode), sm(sm), _pos(pos), _size(size), states(states)
    {}
    void update();
    void draw() const;
    olc::vf2d size() {return _size;}
    olc::vf2d pos() {return _pos;}
    std::optional<olc::vf2d> screen_to_canvas(olc::vf2d screen_pos);
};

class SmeagolGUI : public olc::PixelGameEngine {
    StateManager& sm;
    StateCanvas sc;
    ButtonPanel bp;
    e_mode mode = NORMAL;
    std::unordered_map<StateID, GUIState> states;

public:
    SmeagolGUI(StateManager& sm)
    : sm(sm),
      sc(StateCanvas(*this, mode, sm, {200,0}, {599, 399}, states)),
      bp(ButtonPanel(*this, mode, sm, {0,0}, {199, 399}))
    {
        sAppName = "Smeagol";
    }
public:
    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;
};

void run_gui(StateManager& sm);