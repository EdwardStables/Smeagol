#include <iostream> 

#include "state_manager.h"
#include "config.h"
#include "olcPixelGameEngine.h"
#include "gui.h"

int main(int argc, char **argv) {
    Config config(argc, argv); 

    if (config.run_gui){
        run_gui();
    }

    StateManager sm;

    StateID one = sm.add_state("State 1");
    StateID two = sm.add_state("State 2");
    StateID three = sm.add_state("State 3");
    InputID a = sm.add_input("a");
    InputID b = sm.add_input("b");
    sm.connect(one, two, a);
    sm.connect(one, two, a);
    sm.connect(one, two, b);
    sm.connect(one, three, a);

    std::cout << sm.summary_string() << std::endl;
    
    return 0;
}
