#include "AnnualData.h"

#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

using std::string, std::pair, std::vector, std::ifstream, std::stringstream,
    std::max, std::deque;

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
        if (cells.size() < 3) {
            throw std::runtime_error(
                "All rows must have at least three columns.");
        }
        // skip if time does not match
        if (cells[1] != time) continue;
        // push back year and temperature
        try {
            int year = stoi(cells[0].substr(0, 4));
            if (yrs.empty() || yrs.back() != year) yrs.push_back(year);
            float temperature = stof(cells[2]);
            data[year].push_back(temperature);
        } catch (...) {
            throw std::runtime_error("Unexpected error when converting data.");
        }
    }
    if(data.empty()) {
        throw std::runtime_error("No entries found for provided time.");
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
        result.push_back(0);
        deque<int> low, high;
        int start = 0;
        for (int end = 0; end < static_cast<int>(data[year].size()); end++) {
            // high stores indices of maximums
            while (!high.empty() && data[year][high.back()] <= data[year][end])
                high.pop_back();
            high.push_back(end);
            // low stores indices of minimums
            while (!low.empty() && data[year][low.back()] >= data[year][end])
                low.pop_back();
            low.push_back(end);
            // move start if amplitude is too large
            while (data[year][high.front()] - data[year][low.front()] > width) {
                start++;
                if (high.front() < start) high.pop_front();
                if (low.front() < start) low.pop_front();
            }
            // update length
            result.back() = max(result.back(), end - start + 1);
        }
    }
    return result;
}