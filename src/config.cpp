#include <iostream> 
#include <getopt.h>

#include "config.h"

void Config::print_help(){
    std::cerr<< "Usage: " << argv[0] << " [-h]\n";
    std::cerr<< "Smeagol is a State Machine visualiser, simulator, and code generator." << std::endl;
    std::cerr<< "    -g    Run PGE GUI" << std::endl;
    std::cerr<< "    -h    Show this message" << std::endl;
}
    
void Config::handle_arguments(){
    int opt;
    while ((opt = getopt(argc, argv, "gh")) != -1){
        switch(opt){
            case 'g':
                run_gui = true;
                break;
            case 'h':
                print_help(); 
                exit(0);
                break;
        }
    }
}