#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <vector>
#include "AnnualData.h"

using std::string, std::pair, std::vector, std::ifstream, std::stringstream,
    std::max, std::iota;

AnnualData::AnnualData(string address, string time) {
// open file and read it line by line
    ifstream f(address);
    string line;
    while (getline(f, line)) {
        // read each cell in the line
        stringstream ss(line);
        string cell;
        vector<string> cells;
        while (getline(ss, cell, ',')) cells.push_back(cell);
        // skip if time does not match
        if (cells[1] != time) continue;
        // push back year and temperature
        int year = stoi(cells[0].substr(0, 4));
        float temperature = stof(cells[2]);
        data.push_back({year, temperature});
    }
    // store first and last years
    firstYear = data.front().first;
    lastYear = data.back().first;
    // close file
    f.close();
}

vector<int> AnnualData::years() {
    vector<int> years(lastYear - firstYear + 1);
    iota(years.begin(), years.end(), firstYear);
    return years;
}

vector<int> AnnualData::count(float low, float high) {
    vector<int> result;
    int currentYear = firstYear - 1;
    for (auto [year, temperature] : data) {
        if (year != currentYear) {
            currentYear = year;
            result.push_back(0);
        }
        if (low <= temperature && temperature <= high) {
            result.back()++;
        }
    }
    return result;
}

vector<int> AnnualData::range(float low, float high) {
    vector<int> result;
    int currentYear = firstYear - 1;
    int streak = 0;
    for (auto [year, temperature] : data) {
        if (year != currentYear) {
            currentYear = year;
            result.push_back(0);
            streak = 0;
        }
        if (low <= temperature && temperature <= high) {
            streak++;
            result.back() = max(result.back(), streak);
        } else {
            streak = 0;
        }
    }
    return result;
}

vector<int> AnnualData::amplitude(float width) {
    // TODO
    vector<int> result;
    result.push_back(width);
    return result;
}