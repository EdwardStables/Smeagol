#include "gui.h"
#include "layout.h"

void GUIState::draw(olc::PixelGameEngine& pge){
    olc::Pixel colour;
    switch (draw_state){
        case HOVER: colour = olc::YELLOW; break;
        case SELECT: colour = olc::DARK_YELLOW; break;
        default: colour = olc::WHITE; break;
    }
    pge.DrawCircle(pos, radius, colour);
    pge.DrawString(pos, state.name);
}

bool GUIState::intersects(olc::vf2d test_pos){
    bool res = (test_pos - pos).mag() < radius - 0.1f;
    return res;
}

void ButtonPanel::update() {
    olc::vi2d offs = {0, 12 + margin.y};
    int ind = 0;
    auto mpos = pge.GetMousePos();
    for (auto &b : buttons){
        olc::vi2d tl = _pos + margin + (ind*offs);
        olc::vi2d br = tl + olc::vi2d{_size.x - 2*margin.x, 12};
        ind++;
        
        if (mpos.x < tl.x
            || mpos.y < tl.y
            || mpos.x > br.x
            || mpos.y > br.y)
        {
            b.draw_state = Button::NONE;
            continue;
        }

        if (!pge.GetMouse(0).bHeld)
            b.draw_state = Button::HOVER;
        else
            b.draw_state = Button::SELECT;

    }
}

void ButtonPanel::draw() {
    olc::vi2d offs = {0, 12 + margin.y};
    int ind = 0;
    for (const auto &b : buttons){
        if (b.draw_state == Button::NONE){
            pge.DrawRect(_pos + margin + (ind*offs), {_size.x - 2*margin.x, 12});
        } else {
            olc::Pixel colour = b.draw_state == Button::HOVER ? olc::GREY : olc::WHITE;
            pge.FillRect(_pos + margin + (ind*offs), {_size.x - 2*margin.x, 12}, colour);
        }
        olc::Pixel text_colour = b.draw_state == Button::NONE ? olc::WHITE : olc::BLACK;
        pge.DrawString(_pos + margin + (ind*offs) + olc::vi2d{2, 2}, b.name, text_colour);
        ind++;
    }

    pge.DrawRect(_pos, _size);
}

std::optional<olc::vf2d> StateCanvas::screen_to_canvas(olc::vf2d screen_pos) {
    olc::vf2d offset = screen_pos - _pos;

    if (offset.x < 0.0f || offset.y < 0.0f) return std::nullopt;
    if (offset.x > _size.x || offset.y > _size.y) return std::nullopt;

    return offset;
}

void StateCanvas::update() {
    // Update    
    if (auto mpos = screen_to_canvas(pge.GetMousePos())){
        for (auto &[id, state] : states){
            if (!(held_id == std::nullopt || held_id.value() == id)) continue;

            if (state.draw_state == GUIState::SELECT &&
                (mpos.value() - state.pos - state.grab_offset).mag2() > 0.01f)
            {
                state.pos = mpos.value() - state.grab_offset; 
            }


            if (state.intersects(mpos.value())){
                state.draw_state = pge.GetMouse(0).bHeld ? GUIState::SELECT : GUIState::HOVER;
                if(pge.GetMouse(0).bPressed || state.draw_state == GUIState::SELECT){
                    held_id = id;
                    state.grab_offset = mpos.value() - state.pos;
                } else if (pge.GetMouse(0).bReleased) {
                    held_id = std::nullopt;
                    state.grab_offset = state.pos;
                }
            } else {
                state.draw_state = GUIState::NONE;
            }
        }
    }
}

void StateCanvas::draw() {
    olc::Sprite state_canvas(_size.x, _size.y);
    pge.SetDrawTarget(&state_canvas);
    for (auto &[id, state] : states){
        state.draw(pge);

        for (const auto &[input_id, target_ids] : sm.transitions.at(id)){
            for (const auto &target_id: target_ids){
                pge.DrawLine(state.pos, states.at(target_id).pos);
            }
        }
    }
    pge.SetDrawTarget(nullptr);
    pge.DrawSprite(_pos, &state_canvas);
    pge.DrawRect(_pos, _size);
}

bool SmeagolGUI::OnUserCreate() {
    for (const auto &[id, state] : sm.states){
        states.insert({id, GUIState({0,0}, 20, state)});
    }

    Layout layout(sm, 3.0f*states.begin()->second.radius, sc.size().x, sc.size().y);

    for (auto &[id, state] : states){
        state.pos = layout.pos(id);
    }

    return true;
}


// Called by olcConsoleGameEngine
bool SmeagolGUI::OnUserUpdate(float fElapsedTime) {
    // Drawing
    Clear(olc::BLACK);
    sc.update();
    bp.update();

    sc.draw();
    bp.draw();

    return true;
}

void run_gui(StateManager& sm)
{
    SmeagolGUI gui(sm);
    if(gui.Construct(800, 400, 2, 2))
        gui.Start();
}

