#pragma once
#include <string>

struct Todo {
    int id;
    std::string title;
    bool completed = false;
    std::string due;
    std::string category;
    std::string description;
};
