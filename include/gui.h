#pragma once

#include "olcPixelGameEngine.h"

class SmeagolGUI : public olc::PixelGameEngine
{
public:
    SmeagolGUI()
    {
        sAppName = "Smeagol";
    }
public:
    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;
};

void run_gui();