#pragma once
#include "Todo.h"
#include <vector>
#include <string>

class TodoStorage {
public:
    static bool loadFromFile(std::vector<Todo> &todos, int &nextId);
    static bool saveToFile(const std::vector<Todo> &todos, int nextId);
};
