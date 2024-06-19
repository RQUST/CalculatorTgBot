#include "data_base/data_base.h"

#include <stdexcept>

Database::Database() : db_(nullptr) {
    // Open or create the database file
    if (const auto err = sqlite3_open("calculator_bot.db", &db_); err != SQLITE_OK) {
        throw std::runtime_error("Cannot open database, error code: " + std::to_string(err));
    }

    // Create the table if it does not exist
    char* err_msg = nullptr;
    const char* create_table_query = "CREATE TABLE IF NOT EXISTS CalculationLogs (Id INTEGER PRIMARY KEY AUTOINCREMENT, Timestamp DATETIME DEFAULT CURRENT_TIMESTAMP, ChatId INTEGER, Expression TEXT)";
    if (sqlite3_exec(db_, create_table_query, nullptr, nullptr, &err_msg) != SQLITE_OK) {
        std::string err_message = "SQL error: ";
        err_message += err_msg;
        sqlite3_free(err_msg);
        sqlite3_close(db_);
        throw std::runtime_error(err_message);
    }
}

Database::~Database() {
    sqlite3_close(db_);
}

void Database::InsertLog(int64_t chat_id, const std::string& expression) {
    const char* insert_query = "INSERT INTO CalculationLogs (ChatId, Expression) VALUES (?, ?)";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db_, insert_query, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare insert statement");
    }

    sqlite3_bind_int64(stmt, 1, chat_id);
    sqlite3_bind_text(stmt, 2, expression.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        throw std::runtime_error("Insert statement execution failed");
    }

    sqlite3_finalize(stmt);
}

std::vector<std::tuple<std::string, std::string>> Database::GetLastTenLogs(int64_t chat_id) {
    std::vector<std::tuple<std::string, std::string>> logs;
    const char* select_query = "SELECT Timestamp, Expression FROM CalculationLogs WHERE ChatId = ? ORDER BY Timestamp DESC LIMIT 10";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db_, select_query, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare select statement");
    }

    sqlite3_bind_int64(stmt, 1, chat_id);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string timestamp(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        std::string expression(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        logs.emplace_back(std::make_tuple(timestamp, expression));
    }

    sqlite3_finalize(stmt);
    return logs;
}
