//1540006

#include "Date.hpp"
#include <array>
#include <string>
#include <iomanip>
#include <sstream>
#include <stdexcept> // for std::runtime_error #include <string>
// Object to represent a date and time
Date::Date(const int year, const int month, const int day)
        : date_ {year, month, day} {
    // Verify the date is valid
    // Will assume any year is valid
    // 1 <= month <= 12
    if ((1 > month) || (12 < month)) {
        throw std::runtime_error("Month outside valid range");
    }
    // Check days is correct for month
    // Start by checking for +ve
    if (1 > day) {
        throw std::runtime_error("Day is less than or equal to zero");
    }
// This is a great example of switch and the utility of fall-through logic
    bool good{false}; // Assume the days is invalid and test for valid values
    switch (day) {

        case 2:
            good = (29 > day); // 28 days or less in non-leap-years
            break;
// 30 days in April, June, September and November
        case 4:
        case 6:
        case 9:
        case 11:
            good = (31 > day);
            break;
        default:
            good = (32 > day);
            break;
    }
    if (!good) {
        throw std::runtime_error("There are less days in the month than that");
    }
}
//function to increment the day by one
void Date::NextDate() {
    int curr_day = date_.at(2);
    int curr_month = date_.at(1);
    int curr_year = date_.at(0);

    int new_month{curr_month};
    int new_year{curr_year};

    int new_day = curr_day+1;
    if(curr_month == 2){
        if(new_day>28){
            new_month = curr_month+1;
            new_day = 01;
       }
        } else if(curr_month==1||curr_month==3||curr_month==5||curr_month==7||curr_month==8||curr_month==10){
        if(new_day>31){
            new_month = curr_month+1;
            new_day = 01;
        }
    } else if(curr_month == 12){
        if(new_day>31){
            new_year = curr_year+1;
            new_month = 01;
            new_day = 01;
        }
    } else if(curr_month==4||curr_month==6||curr_month==9||curr_month==11){
        if(new_day>30){
            new_month = curr_month+1;
            new_day = 01;
        }
    }
    date_.at(0) = new_year;
    date_.at(1) = new_month;
    date_.at(2)= new_day;


}
//function to return the date as int YYYYMMDD format
int Date::GetDate(){
    int year = date_.at(0);
    int month = date_.at(1);
    int day = date_.at(2);
    //strings are used so the YYYY0M0D format can be achieved
    std::string s1{std::to_string(year)}; //year
    std::string s2{std::to_string(month)}; //month
    std::string s3{std::to_string(day)}; //day

    if(month==10||month ==11||month==12){
        if(day==1||day==2||day==3||day==4||day==5||day==6||day==7||day==8||day==9){
            std::string s0{std::to_string(0)};
            s3 =s0+s3;
        }
    }else{
        std::string s0{std::to_string(0)};
        s2 = s0+s2;
        if(day==1||day==2||day==3||day==4||day==5||day==6||day==7||day==8||day==9){
            s3 =s0+s3;
        }
    }

    std::string s4{s1+s2+s3}; //total
    int out{std::stoi(s4)};
    return out; //format YYYYMMDD
}
//function to return a vector with elements year,month,day
std::vector<int> Date::Singles(){ //current date as a vector
    std::vector<int>dates;
    dates.push_back(date_.at(2));
    dates.push_back(date_.at(1));
    dates.push_back(date_.at(0));
    return dates;
}
//function that adds a year
void Date::AddYear(int years){
    int curr_year = date_.at(0);
    date_.at(0) = curr_year+years;

}
//used to calculate tie in end date
int CalcDate(Date temp_dateobj,int tie_in){
    temp_dateobj.AddYear(tie_in);
    int future_date = temp_dateobj.GetDate();

    return future_date;
}
// calculates the number of days between dates
int CalcDays(Date currobj, Date transobj){
    int count{0};
    const int end_date = currobj.GetDate();
    int curr_date{transobj.GetDate()};

    while(end_date != curr_date){
        transobj.NextDate();
        curr_date = transobj.GetDate();
        count++;
    }
    return count;
}
// converts YYYYMMDD into years months and days
std::vector<int> DecompDate(int date){
    std::stringstream ss;
    ss<<date;
    int year = std::atoi(ss.str().substr(0,4).c_str());
    int month = std::atoi(ss.str().substr(4,2).c_str());
    int day = std::atoi(ss.str().substr(6,2).c_str());
    std::vector<int> vec;
    vec.emplace_back(year);
    vec.emplace_back(month);
    vec.emplace_back(day);

    return vec;

}

