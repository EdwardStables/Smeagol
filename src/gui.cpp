#include "gui.h"

bool SmeagolGUI::OnUserCreate() {
    return true;
}


// Called by olcConsoleGameEngine
bool SmeagolGUI::OnUserUpdate(float fElapsedTime) {
    // Clear Screen
    Clear(olc::BLACK);

    DrawString({0,0}, sm.summary_string());

    return true;
}

void run_gui(StateManager& sm)
{
    SmeagolGUI gui(sm);
    if(gui.Construct(800, 400, 2, 2))
        gui.Start();
}

