#pragma once

using InputID = uint32_t;

class InputBase {
public:
    const std::string name;
    const InputID id;

    InputBase(std::string name, InputID id) :
        name(name),
        id(id)
    {}

};
