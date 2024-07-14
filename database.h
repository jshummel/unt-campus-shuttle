// database.h
#ifndef DATABASE_H
#define DATABASE_H

#include <unordered_map>
#include <vector>
#include <string>
#include <filesystem>

using namespace std;

class Database {
public:
    Database();
    ~Database();

    unordered_map<string, unordered_map<string, vector<string>>> routeDB;

    unordered_map<string, unordered_map<string, vector<string>>> stopDB;

    vector<string> allBusStopNames;

    void printDatabase(const unordered_map<string, unordered_map<string, vector<string>>>& database);

    void printList(const vector<string>& list);

private:
    void printMap(const unordered_map<string, vector<string>>& map);

    unordered_map<string, vector<string>> csvParser(const filesystem::path& filepath, vector<string>& allBusStopNames);
};

#endif // DATABASE_H
