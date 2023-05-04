#include "gui.h"
#include "layout.h"

void GUIState::draw(olc::PixelGameEngine& pge){
    pge.DrawCircle(pos, radius);
    pge.DrawString(pos, state.name);
}

bool SmeagolGUI::OnUserCreate() {
    for (const auto &[id, state] : sm.states){
        states.insert({id, GUIState({0,0}, 20, state)});
    }

    Layout layout(sm, 3.0f*states.begin()->second.radius, state_zone_size.x, state_zone_size.y);

    for (auto &[id, state] : states){
        state.pos = layout.pos(id);
    }

    return true;
}


// Called by olcConsoleGameEngine
bool SmeagolGUI::OnUserUpdate(float fElapsedTime) {
    // Clear Screen
    Clear(olc::BLACK);

    olc::Sprite state_canvas(state_zone_size.x, state_zone_size.y);
    SetDrawTarget(&state_canvas);
    for (auto &[id, state] : states){
        state.draw(*this);

        for (const auto &[input_id, target_ids] : sm.transitions.at(id)){
            for (const auto &target_id: target_ids){
                DrawLine(state.pos, states.at(target_id).pos);
            }
        }
    }
    SetDrawTarget(nullptr);
    DrawSprite(state_zone_start, &state_canvas);
    DrawRect(state_zone_start, state_zone_size);

    return true;
}

void run_gui(StateManager& sm)
{
    SmeagolGUI gui(sm);
    if(gui.Construct(800, 400, 2, 2))
        gui.Start();
}

