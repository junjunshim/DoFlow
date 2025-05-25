#pragma once
#include "Todo.h"
#include <vector>

class TodoRepository {
public:
    static void initDB();                         // 테이블 없으면 생성
    static std::vector<Todo> getAll();            // 전체 할 일 불러오기
};
