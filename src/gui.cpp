#include "gui.h"

void GUIState::draw(olc::PixelGameEngine& pge){
    pge.DrawCircle(pos, radius);
    pge.DrawString(pos, state.name);
}

bool SmeagolGUI::OnUserCreate() {
    for (const auto &[id, state] : sm.states){
        states.push_back(GUIState({0,0}, 20, state));
    }


    olc::vf2d offs = {0,0};
    for (auto &state : states){
        state.pos += offs;
        offs += {100,100};
    }

    return true;
}


// Called by olcConsoleGameEngine
bool SmeagolGUI::OnUserUpdate(float fElapsedTime) {
    // Clear Screen
    Clear(olc::BLACK);

    for (auto &state : states){
        state.draw(*this);
    }

    return true;
}

void run_gui(StateManager& sm)
{
    SmeagolGUI gui(sm);
    if(gui.Construct(800, 400, 2, 2))
        gui.Start();
}

