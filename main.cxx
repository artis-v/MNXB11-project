#include <iostream>
#include <AnnualData.h>

using std::cout, std::endl;

int main() { // int argc, char *argv[]
    AnnualData ad("datasets/Boras_Data_Cleaned.csv", "18:00:00");
    // Some examples:
    for (auto el : ad.years()) {
        cout << el << " ";
    }
    cout << endl;
    for (auto el : ad.count(16.6, 18.1)) {
        cout << el << " ";
    }
    cout << endl;
    for (auto el : ad.range(16.6, 18.1)) {
        cout << el << " ";
    }
    cout << endl;
    for (auto el : ad.amplitude(1.5)) {
        cout << el << " ";
    }
    cout << endl;
}
