//
// 1540006
//

#ifndef PROJECT2_DATE_HPP
#define PROJECT2_DATE_HPP

#include <iostream>
#include <array>
#include "StoreInput.hpp"

class Date {
private:
    // Array to store the year, month, day, hours, minutes, seconds
    std::array<int, 3> date_;
public:
// Constructor, takes year, month, day, hour, minute, second as arguments
    Date(const int year, const int month, const int day); // Returns a string representation of the date in form "Y-M-D"
    ~Date() = default;
    int GetDate() ;
    std::vector<int> Singles();
    void NextDate();

    void AddYear(int years);

};

int CalcDate(Date,int);
int CalcDays(Date currobj, Date transobj);
std::vector<int> DecompDate(int);
#endif //PROJECT2_DATE_HPP
