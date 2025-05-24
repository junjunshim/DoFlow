#pragma once
#include <drogon/HttpController.h>
#include "../models/Todo.h"

using namespace drogon;

class TodoController : public HttpController<TodoController> {
public:
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(TodoController::getTodos, "/todos", Get); //조회
        ADD_METHOD_TO(TodoController::createTodo, "/todos", Post); //추가
        ADD_METHOD_TO(TodoController::updateTodo, "/todos/{1}", Put);    // 수정
        ADD_METHOD_TO(TodoController::deleteTodo, "/todos/{1}", Delete); // 삭제
        ADD_METHOD_TO(TodoController::resetTodos, "/todos/reset", Delete); // 초기화
    METHOD_LIST_END


    void getTodos(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
    void createTodo(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
    void updateTodo(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int id);
    void deleteTodo(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int id);
    void resetTodos(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
};

