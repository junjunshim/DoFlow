#include "TodoStorage.h"
#include <json/json.h>
#include <fstream>

bool TodoStorage::loadFromFile(std::vector<Todo> &todos, int &nextId) {
    std::ifstream in("todos.json", std::ifstream::binary);
    if (!in.is_open()) return false;

    Json::Value root;
    in >> root;
    in.close();

    todos.clear();
    for (const auto &item : root["todos"]) {
        Todo todo;
        todo.id = item["id"].asInt();
        todo.title = item["title"].asString();
        todo.completed = item["completed"].asBool();
        todo.due = item.get("due", "").asString();
        todo.category = item.get("category", "").asString();
        todo.description = item.get("description", "").asString();
        todos.push_back(todo);
    }

    nextId = root.get("nextId", todos.size() + 1).asInt();
    return true;
}

bool TodoStorage::saveToFile(const std::vector<Todo> &todos, int nextId) {
    Json::Value root;
    root["nextId"] = nextId;

    for (const auto &todo : todos) {
        Json::Value item;
        item["id"] = todo.id;
        item["title"] = todo.title;
        item["completed"] = todo.completed;
        item["due"] = todo.due;
        item["category"] = todo.category;
        item["description"] = todo.description;
        root["todos"].append(item);
    }

    std::ofstream out("todos.json");
    if (!out.is_open()) return false;

    out << root;
    out.close();
    return true;
}
