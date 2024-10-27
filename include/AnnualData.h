#ifndef __ANNUALDATA_H__
#define __ANNUALDATA_H__

#include <iostream>
#include <vector>
#include <map>
#include <set>

using std::string, std::vector, std::pair, std::map, std::set;

class AnnualData {
   public:
    AnnualData(string address, string time);
    vector<int> years();
    vector<int> count(float low, float high);
    vector<int> range(float low, float high);
    vector<int> amplitude(float width);

   private:
    vector<int> yrs;
    map<int, vector<float>> data;
};

#endif