//
// 1540006
//
#include <iostream>
#include <vector>
#include <string>

#ifndef PROJECT2_STOREINPUT_HPP
#define PROJECT2_STOREINPUT_HPP

void print(std::vector<std::string>);
void print(std::vector<int>);

struct ClientData{
    std::vector<std::string> Clients;
    std::vector<int> Cap;
    std::vector<int> Ret;
    ~ClientData() = default;
};


struct ProdData{
    std::vector<std::string> Comp;
    std::vector<std::string> Prod;
    std::vector<int> AER;
    std::vector<std::string> AM;
    std::vector<int> tie_in;
    std::vector<int> min_invest;
    std::vector<int> max_invest;
    std::vector<int> Avail;
    std::vector<int> Mat;
    ~ProdData() = default;
};
int input_checker(const std::string);
int input_checker_feed(const std::string);
ClientData readin(std::string, int n);
ProdData readin_feed(std::string, int n);

class StoreInput {
protected:

//Client Data
     std::vector<std::string> clients_;
     std::vector<int> cap_;
     std::vector<int> ret_;

    //Feed Data
     std::vector<std::string> comp_;
     std::vector<std::string> prod_;
     std::vector<int> aer_;
     std::vector<std::string> am_;
     std::vector<int> tie_in_;
     std::vector<int> min_invest_;
     std::vector<int> max_invest_;
     std::vector<int> avail_;
     std::vector<int> mat_;

public:
    //constructor
    StoreInput(std::vector<std::string> Clients, std::vector<int> Cap, std::vector<int> Ret,
               std::vector<std::string> Comp, std::vector<std::string> Prod, std::vector<int> AER,
               std::vector<std::string> AM, std::vector<int> tie_in, std::vector<int> min_invest,
               std::vector<int> max_invest, std::vector<int> Avail, std::vector<int> Mat);
    ~StoreInput() = default;

};


#endif //PROJECT2_STOREINPUT_HPP
