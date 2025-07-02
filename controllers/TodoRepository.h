#pragma once
#include "Todo.h"
#include <vector>

class TodoRepository {
public:
    static void initDB();                         // 데이터 테이블 생성
    static std::vector<Todo> getAll();            // 전체 할 일 불러오기
    static int add(const Todo &todo);             // 리스트 추가 & 성공 시 ID 반환
    static bool update(const Todo &todo);         // 리스트 수정
    static bool remove(int id);                   // 리스트 삭제
};
