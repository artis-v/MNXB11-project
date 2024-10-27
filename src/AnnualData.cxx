#include "AnnualData.h"

#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <vector>

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
        if (yrs.empty() || yrs.back() != year) yrs.push_back(year);
        float temperature = stof(cells[2]);
        data[year].push_back(temperature);
    }
    // close file
    f.close();
}

vector<int> AnnualData::years() { return yrs; }

vector<int> AnnualData::count(float low, float high) {
    vector<int> result;
    for (int year : yrs) {
        result.push_back(0);
        for (float temperature : data[year]) {
            if (low <= temperature && temperature <= high) {
                result.back()++;
            }
        }
    }
    return result;
}

vector<int> AnnualData::range(float low, float high) {
    vector<int> result;
    for (int year : yrs) {
        result.push_back(0);
        int streak = 0;
        for (float temperature : data[year]) {
            if (low <= temperature && temperature <= high) {
                streak++;
                result.back() = max(result.back(), streak);
            } else {
                streak = 0;
            }
        }
    }
    return result;
}

vector<int> AnnualData::amplitude(float width) {
    vector<int> result;
    for (int year : yrs) {
        result.push_back(year - static_cast<int>(width));
    }
    return result;
}