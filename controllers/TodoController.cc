#include "TodoController.h"
#include "TodoStorage.h"
#include "TodoRepository.h"
#include <json/json.h>
#include <vector>

//데이터 불러오기 옵션(기본 splite)
//#define JSON

static std::vector<Todo> todos;
int nextId = 1;

// ✅ 서버 시작 시 자동 로딩
struct TodoInitializer {
    TodoInitializer() {
        //데이터 불러오기 옵션에 맞게 작동 JSON or SPLite
        #ifdef JSON
            if (TodoStorage::loadFromFile(todos, nextId)) {
                std::cout << "[INFO] 할 일 목록이 todos.json에서 복원되었습니다. 총 " << todos.size() << "개\n";
            } else {
                std::cout << "[INFO] todos.json이 없거나 비어 있습니다. 새 목록으로 시작합니다.\n";
            }
        #else
            TodoRepository::initDB();
            todos = TodoRepository::getAll();
            std::cout << "[INFO] DB에서 불러온 할 일 수: " << todos.size() << "\n";
        #endif
    }   
};

// ✅ 전역 인스턴스 → 프로그램 시작 시 생성됨
static TodoInitializer _todoLoader;


//getTodos함수는 json방식과 splite 방식 모두 동일하게 작동
void TodoController::getTodos(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    Json::Value arr(Json::arrayValue);
    for (const auto &todo : todos) {
        Json::Value item;
        item["id"] = todo.id;
        item["title"] = todo.title;
        item["completed"] = todo.completed;   
        item["due"] = todo.due;
        item["category"] = todo.category;
        item["description"] = todo.description;
        arr.append(item);
    }
    callback(HttpResponse::newHttpJsonResponse(arr));
}

void TodoController::createTodo(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    auto json = req->getJsonObject();
    if (!json || !json->isMember("title")) {
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k400BadRequest);
        resp->setBody("Missing title");
        return callback(resp);
    }

    Todo todo;
    todo.id = nextId++; //Json 방식
    todo.title = (*json)["title"].asString();
    todo.completed = false;
    todo.due = json->isMember("due") ? (*json)["due"].asString() : "";
    todo.category = json->isMember("category") ? (*json)["category"].asString() : "";
    todo.description = json->isMember("description") ? (*json)["description"].asString() : "";
    todos.push_back(todo);

    #ifdef JSON

    TodoStorage::saveToFile(todos, nextId);

    Json::Value result;
    result["id"] = todo.id;
    result["title"] = todo.title;
    result["completed"] = todo.completed;
    result["due"] = todo.due;
    callback(HttpResponse::newHttpJsonResponse(result));

    #else

    int newId = TodoRepository::add(todo);
    if (newId <= 0) {
        auto err = HttpResponse::newHttpResponse();
        err->setStatusCode(k500InternalServerError);
        err->setContentTypeCode(CT_TEXT_PLAIN);
        err->setBody("Failed to insert todo into database.");
        callback(err);
        return;
    }
    
    Json::Value result;
    result["id"] = newId;
    auto resp = HttpResponse::newHttpJsonResponse(result);
    callback(resp);

    #endif
}

void TodoController::updateTodo(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int id) {
    auto json = req->getJsonObject();
    if (!json || (!json->isMember("title") && !json->isMember("completed") && !json->isMember("due"))) {
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k400BadRequest);
        resp->setBody("Missing fields to update");
        return callback(resp);
    }
    if(json->isMember("title")){
        if (!(*json)["title"].isString() || (*json)["title"].asString().empty()) {
            auto resp = HttpResponse::newHttpResponse();
            resp->setStatusCode(k400BadRequest);
            resp ->setBody("Empty title field");
            callback(resp);
            return;
        }
    }
    

    #ifdef JSON
    for (auto &todo : todos) {
        if (todo.id == id) {
            if (json->isMember("title") && (*json)["title"].isString()){
                todo.title = (*json)["title"].asString();
            }
            if (json->isMember("completed") && (*json)["completed"].isBool()){
                todo.completed = (*json)["completed"].asBool();
            }
            if (json->isMember("due") && (*json)["due"].isString()) {
                todo.due = (*json)["due"].asString();
            }
            
            TodoStorage::saveToFile(todos, nextId);

            Json::Value result;
            result["id"] = todo.id;
            result["title"] = todo.title;
            result["completed"] = todo.completed;
            result["due"] = todo.due;
            return callback(HttpResponse::newHttpJsonResponse(result));
        }
    }

    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(k404NotFound);
    resp->setBody("Todo not found");
    callback(resp);

    #else
    
    Todo todo;
    todo.id = id;
    todo.title = (*json)["title"].asString();
    todo.completed = json->isMember("completed") ? (*json)["completed"].asBool() : false;
    todo.due = json->isMember("due") ? (*json)["due"].asString() : "";
    todo.category = json->isMember("category") ? (*json)["category"].asString() : "";
    todo.description = json->isMember("description") ? (*json)["description"].asString() : "";

    if (!TodoRepository::update(todo)) {
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k404NotFound);
        resp->setBody("Todo not found or update failed.");
        callback(resp);
        return;
    }

    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(k200OK);
    resp->setContentTypeCode(CT_TEXT_PLAIN);
    resp->setBody("Updated");
    callback(resp);

    #endif
}

void TodoController::deleteTodo(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int id) {
    for (auto it = todos.begin(); it != todos.end(); ++it) {
        if (it->id == id) {
            todos.erase(it);

            TodoStorage::saveToFile(todos, nextId);

            auto resp = HttpResponse::newHttpResponse();
            resp->setBody("Todo deleted");
            return callback(resp);
        }
    }

    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(k404NotFound);
    resp->setBody("Todo not found");
    callback(resp);
}

void TodoController::resetTodos(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    todos.clear();
    nextId = 1;

    TodoStorage::saveToFile(todos, nextId);

    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(k200OK);
    resp->setContentTypeCode(CT_TEXT_PLAIN);
    resp->setBody("All todos deleted.");
    callback(resp);
}
