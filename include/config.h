#pragma once

struct Config {
    int argc;
    char **argv;
    void print_help();
    void handle_arguments();
    Config(int argc, char **argv) : argc(argc), argv(argv) {
        handle_arguments();
    }
};