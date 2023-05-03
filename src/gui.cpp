#include "gui.h"

bool SmeagolGUI::OnUserCreate() {
    return true;
}


// Called by olcConsoleGameEngine
bool SmeagolGUI::OnUserUpdate(float fElapsedTime) {
    // Clear Screen
    Clear(olc::BLACK);
    return true;
}

void run_gui()
{
    SmeagolGUI gui;
    if(gui.Construct(800, 400, 2, 2))
        gui.Start();
}

