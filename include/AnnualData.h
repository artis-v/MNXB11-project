#ifndef __ANNUALDATA_H__
#define __ANNUALDATA_H__

#include <iostream>
#include <vector>

using std::string, std::vector, std::pair;

class AnnualData {
   public:
    AnnualData(string address, string time);
    vector<int> years();
    vector<int> count(float low, float high);
    vector<int> range(float low, float high);
    vector<int> amplitude(float width);

   private:
    int firstYear, lastYear;
    vector<pair<int, float>> data;
};

#endif