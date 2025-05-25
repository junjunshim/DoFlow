#include "TodoRepository.h"
#include <sqlite3.h>
#include <iostream>

static const char *DB_PATH = "todos.db";

void TodoRepository::initDB() {
    sqlite3 *db;
    if (sqlite3_open(DB_PATH, &db) != SQLITE_OK) {
        std::cerr << "DB 열기 실패: " << sqlite3_errmsg(db) << "\n";
        return;
    }

    const char *sql =
        "CREATE TABLE IF NOT EXISTS todos ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "title TEXT NOT NULL,"
        "completed INTEGER DEFAULT 0,"
        "due TEXT,"
        "category TEXT,"
        "description TEXT"
        ");";

    char *errMsg = nullptr;
    if (sqlite3_exec(db, sql, 0, 0, &errMsg) != SQLITE_OK) {
        std::cerr << "테이블 생성 오류: " << errMsg << "\n";
        sqlite3_free(errMsg);
    }

    sqlite3_close(db);
}

std::vector<Todo> TodoRepository::getAll() {
    std::vector<Todo> result;
    sqlite3 *db;
    if (sqlite3_open(DB_PATH, &db) != SQLITE_OK) {
        std::cerr << "DB 열기 실패\n";
        return result;
    }

    const char *sql = "SELECT id, title, completed, due, category, description FROM todos;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "쿼리 준비 실패\n";
        sqlite3_close(db);
        return result;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Todo todo;
        todo.id = sqlite3_column_int(stmt, 0);
        todo.title = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        todo.completed = sqlite3_column_int(stmt, 2);
        todo.due = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
        todo.category = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
        todo.description = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5));
        result.push_back(todo);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return result;
}

int TodoRepository::add(const Todo &todo) {
    sqlite3 *db;
    if (sqlite3_open(DB_PATH, &db) != SQLITE_OK) {
        std::cerr << "DB 열기 실패\n";
        return -1;
    }

    const char *sql =
        "INSERT INTO todos (title, completed, due, category, description) "
        "VALUES (?, ?, ?, ?, ?);";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "INSERT 준비 실패\n";
        sqlite3_close(db);
        return -1;
    }

    sqlite3_bind_text(stmt, 1, todo.title.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, todo.completed ? 1 : 0);
    sqlite3_bind_text(stmt, 3, todo.due.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, todo.category.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, todo.description.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "INSERT 실패\n";
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return -1;
    }

    int insertedId = static_cast<int>(sqlite3_last_insert_rowid(db));

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return insertedId;
}

