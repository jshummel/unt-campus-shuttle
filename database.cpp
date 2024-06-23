// database.cpp
#include "database.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <QDebug>

Database::Database() {
    string dPath = "_CSV";
    unordered_map<string, vector<string>> busRoute; // key=busStopName and value=busStopTimes, loaded into routeDB value

    for (const auto& file : filesystem::directory_iterator(dPath)) {
        if (file.is_regular_file()) {
            busRoute = csvParser(file.path(), allBusStopNames);
        }
        string busRouteName = file.path().string(); // loaded into routeDB key
        busRouteName = busRouteName.substr(5);
        busRouteName = busRouteName.erase(busRouteName.length() - 4);
        routeDB[busRouteName] = busRoute;
    }

    // use routeDB components to init stopDB
    for (const auto& pair : routeDB) {
        string busRouteName = pair.first;
        unordered_map<string, vector<string>> busStops = pair.second;

        for (const auto& pair : busStops) {
            string busStopName = pair.first;
            vector<string> busStopTimes = pair.second;

            stopDB[busStopName][busRouteName] = busStopTimes;
        }
    }
}

Database::~Database() {
    // Destructor logic if needed
}

void Database::printDatabase(const unordered_map<string, unordered_map<string, vector<string>>>& database) {
    for (const auto& pair : database) {
        qInfo() << pair.first.c_str();
        printMap(pair.second);
        qInfo();
    }
}

void Database::printList(const vector<string>& list) {
    for (const string& e : list) {
        qInfo() << e.c_str();
    }
}

void Database::printMap(const unordered_map<string, vector<string>>& map) {
    for (const auto& pair : map) {
        qInfo() << "\t" << pair.first.c_str();
        for (const string& e : pair.second) {
            qInfo() << "\t\t" << e.c_str();
        }
        qInfo();
    }
}

unordered_map<string, vector<string>> Database::csvParser(const filesystem::path& filepath, vector<string>& allBusStopNames) {
    try {
        ifstream file(filepath);
        if (!file.is_open()) {
            throw runtime_error("Error opening " + filepath.string());
        }
        unordered_map<string, vector<string>> stopNameStopTimeMap; // key=busStopName and value=busStopTimes
        string line;
        bool firstLine = true;

        while (getline(file, line)) {
            if (firstLine) {
                firstLine = false;
                if (line.size() >= 3 && static_cast<unsigned char>(line[0]) == 0xEF && static_cast<unsigned char>(line[1]) == 0xBB && static_cast<unsigned char>(line[2]) == 0xBF) {
                    line = line.substr(3);
                }
            }

            stringstream input(line);
            string busStopName; // loaded into map key
            string time;
            vector<string> busStopTimes; // list of all bus stop times to be loaded into map value

            // first element of parsed line is busStopName, all others are times of busStopTimes vector
            while (getline(input, busStopName, ',')) {
                if (find(allBusStopNames.begin(), allBusStopNames.end(), busStopName) == allBusStopNames.end()) {
                    allBusStopNames.push_back(busStopName);
                }

                while (getline(input, time, ',')) {
                    busStopTimes.push_back(time);
                }

                stopNameStopTimeMap[busStopName] = busStopTimes;
                busStopTimes.clear();
            }
        }

        file.close();
        return stopNameStopTimeMap;
    } catch (const exception& ex) {
        qInfo() << ex.what();
        return {};
    }
}
