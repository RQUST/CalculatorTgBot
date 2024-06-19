#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <string>
#include <vector>
#include <tuple>

class Database {
private:
    sqlite3* db_;

public:
    Database();
    ~Database();

    void InsertLog(int64_t chat_id, const std::string& expression);
    std::vector<std::tuple<std::string, std::string>> GetLastTenLogs(int64_t chat_id);
};

#endif // DATABASE_H
