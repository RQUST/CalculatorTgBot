#include "data_base/data_base.h"
#include <stdexcept>
#include <iostream>

Database::Database() : db_(nullptr) {
  if (const auto err = sqlite3_open("calculator_bot.db", &db_); err != SQLITE_OK) {
    throw std::runtime_error("Cannot open database, error code: " + std::to_string(err));
  }

  char* err_msg = nullptr;

  // Удаляем старую таблицу (если существует)
  const char* drop_table_query = "DROP TABLE IF EXISTS CalculationLogs";
  if (sqlite3_exec(db_, drop_table_query, nullptr, nullptr, &err_msg) != SQLITE_OK) {
    std::string err_message = "SQL error while dropping table: ";
    err_message += err_msg;
    sqlite3_free(err_msg);
    sqlite3_close(db_);
    throw std::runtime_error(err_message);
  }

  // Создаем новую таблицу
  const char* create_table_query =
      "CREATE TABLE IF NOT EXISTS CalculationLogs ("
      "Id INTEGER PRIMARY KEY AUTOINCREMENT, "
      "Timestamp DATETIME DEFAULT CURRENT_TIMESTAMP, "
      "ChatId INTEGER, "
      "Result INTEGER)";
  if (sqlite3_exec(db_, create_table_query, nullptr, nullptr, &err_msg) != SQLITE_OK) {
    std::string err_message = "SQL error: ";
    err_message += err_msg;
    sqlite3_free(err_msg);
    sqlite3_close(db_);
    throw std::runtime_error(err_message);
  }
}

Database::~Database() {
  if (db_) {
    sqlite3_close(db_);
  }
}

void Database::InsertLog(int64_t chat_id, int result) {
  static const char* insert_query = "INSERT INTO CalculationLogs (ChatId, Result) VALUES (?, ?)";
  sqlite3_stmt* stmt = nullptr;

  if (sqlite3_prepare_v2(db_, insert_query, -1, &stmt, nullptr) != SQLITE_OK) {
    std::string err_message = "Failed to prepare insert statement: ";
    err_message += sqlite3_errmsg(db_);
    throw std::runtime_error(err_message);
  }

  if (sqlite3_bind_int64(stmt, 1, chat_id) != SQLITE_OK || sqlite3_bind_int(stmt, 2, result) != SQLITE_OK) {
    std::string err_message = "Failed to bind values: ";
    err_message += sqlite3_errmsg(db_);
    sqlite3_finalize(stmt);
    throw std::runtime_error(err_message);
  }

  if (sqlite3_step(stmt) != SQLITE_DONE) {
    std::string err_message = "Insert statement execution failed: ";
    err_message += sqlite3_errmsg(db_);
    sqlite3_finalize(stmt);
    throw std::runtime_error(err_message);
  }

  sqlite3_finalize(stmt);
}

std::vector<std::tuple<std::string, int>> Database::GetLastTenLogs(int64_t chat_id) const {
  std::vector<std::tuple<std::string, int>> logs;
  static const char* select_query =
      "SELECT Timestamp, Result FROM CalculationLogs WHERE ChatId = ? ORDER BY Timestamp DESC LIMIT 10";
  sqlite3_stmt* stmt = nullptr;

  if (sqlite3_prepare_v2(db_, select_query, -1, &stmt, nullptr) != SQLITE_OK) {
    std::string err_message = "Failed to prepare select statement: ";
    err_message += sqlite3_errmsg(db_);
    throw std::runtime_error(err_message);
  }

  if (sqlite3_bind_int64(stmt, 1, chat_id) != SQLITE_OK) {
    std::string err_message = "Failed to bind value: ";
    err_message += sqlite3_errmsg(db_);
    sqlite3_finalize(stmt);
    throw std::runtime_error(err_message);
  }

  while (sqlite3_step(stmt) == SQLITE_ROW) {
    std::string timestamp(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
    int result = sqlite3_column_int(stmt, 1);
    logs.emplace_back(std::make_tuple(timestamp, result));
  }

  if (sqlite3_errcode(db_) != SQLITE_DONE && sqlite3_errcode(db_) != SQLITE_ROW) {
    std::string err_message = "Failed to retrieve logs: ";
    err_message += sqlite3_errmsg(db_);
    sqlite3_finalize(stmt);
    throw std::runtime_error(err_message);
  }

  sqlite3_finalize(stmt);
  return logs;
}
