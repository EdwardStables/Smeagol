#pragma once

#include "olcPixelGameEngine.h"
#include "state_manager.h"

class SmeagolGUI : public olc::PixelGameEngine {

    StateManager& sm;

public:
    SmeagolGUI(StateManager& sm) : sm(sm) {
        sAppName = "Smeagol";
    }
public:
    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;
};

void run_gui(StateManager& sm);