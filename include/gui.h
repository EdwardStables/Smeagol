#pragma once

#include <optional>

#include "olcPixelGameEngine.h"
#include "state_manager.h"
#include "state.h"
#include "inputs.h"

enum e_mode {
    NORMAL, DELETE_STATE, CONNECT_STATE
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
    olc::vi2d _pos;
    olc::vi2d _size;
    e_button_type type;
    enum {NONE, HOVER, SELECT} draw_state = NONE;
    bool button_pressed = false;

    //kinda a hack
    olc::vi2d mpos_offset = {0,0};

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
            {"Remove Mode", Button::TOGGLE},
            {"Connect Mode", Button::TOGGLE}
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

class InputMenu {
    olc::PixelGameEngine& pge;
    e_mode& mode;
    StateManager& sm;
    const olc::vi2d _pos;
    const olc::vi2d _size;

    std::vector<Button> input_buttons;
    std::vector<Button> control_buttons;
    const olc::vi2d margin = {5,5};

    std::vector<InputID> ordered_ids;
    std::optional<int> selected_id;

public:
    InputMenu(
        olc::PixelGameEngine& pge,
        e_mode& mode,
        StateManager& sm,
        olc::vi2d pos,
        olc::vi2d size
    ) : pge(pge), mode(mode), sm(sm), _pos(pos), _size(size)
    {
        olc::vi2d offs = {0, 12 + margin.y};
        int ind = 0;
        control_buttons.push_back(Button(pge, margin + (ind*offs), {_size.x - 2*margin.x, 12}, "+", Button::MOMENTRY));
        control_buttons.back().mpos_offset = -_pos;
    }
    void update();
    void draw() const;
    olc::vf2d size() {return _size;}
    olc::vf2d pos() {return _pos;}
    std::optional<olc::vf2d> screen_to_canvas(olc::vf2d screen_pos) const;
    std::optional<StateID> selected_input() const {
        if (!selected_id) return std::nullopt;
        return ordered_ids[selected_id.value()];
    }
};

class StateCanvas {
    olc::PixelGameEngine& pge;
    e_mode& mode;
    StateManager& sm;
    InputMenu& im;
    std::unordered_map<StateID, GUIState>& states;
    const olc::vi2d _pos;
    const olc::vi2d _size;

    std::optional<StateID> held_id = std::nullopt;
    std::optional<StateID> connection_mode_start = std::nullopt;

public:
    StateCanvas(
        olc::PixelGameEngine& pge,
        e_mode& mode,
        StateManager& sm,
        InputMenu& im,
        olc::vi2d pos,
        olc::vi2d size,
        std::unordered_map<StateID, GUIState>& states
    ) : pge(pge), mode(mode), sm(sm), im(im), _pos(pos), _size(size), states(states)
    {}
    void update();
    void draw() const;
    olc::vf2d size() {return _size;}
    olc::vf2d pos() {return _pos;}
    std::optional<olc::vf2d> screen_to_canvas(olc::vf2d screen_pos) const;
};

class SmeagolGUI : public olc::PixelGameEngine {
    StateManager& sm;
    ButtonPanel bp;
    InputMenu im;
    StateCanvas sc;
    e_mode mode = NORMAL;
    std::unordered_map<StateID, GUIState> states;

public:
    SmeagolGUI(StateManager& sm)
    : sm(sm),
      bp(ButtonPanel(*this, mode, sm, {0,0}, {199, 399})),
      im(InputMenu(*this, mode, sm, {600,0}, {199, 399})),
      sc(StateCanvas(*this, mode, sm, im, {200,0}, {399, 399}, states))
    {
        sAppName = "Smeagol";
    }
public:
    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;
};

void run_gui(StateManager& sm);

float point_to_line_distance(olc::vf2d line_point1, olc::vf2d line_point2, olc::vf2d test_point);