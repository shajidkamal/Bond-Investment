//
// 1540006
//

#ifndef PROJECT2_ALGORITHM_HPP
#define PROJECT2_ALGORITHM_HPP

#include <utility>
#include <vector>
#include "Date.hpp"



class Algorithm: public StoreInput {
public:
    Algorithm(std::vector<std::string> Clients,std::vector<int> Cap, std::vector<int> Ret,
              std::vector<std::string> Comp,std::vector<std::string> Prod,std::vector<int> AER,
              std::vector<std::string> AM,std::vector<int> tie_in,std::vector<int> min_invest,
              std::vector<int> max_invest, std::vector<int> Avail,std::vector<int> Mat)
              : StoreInput(std::move(Clients),std::move(Cap),std::move(Ret),std::move(Comp),std::move(Prod),std::move(AER),std::move(AM),std::move(tie_in),std::move(min_invest),std::move(max_invest),std::move(Avail),std::move(Mat)){};

    std::vector<std::string> GetClient() {return clients_;};
    std::vector<int> GetCap(){return cap_;};
    std::vector<int> GetRet(){return ret_;};
    std::vector<std::string> GetComp(){return comp_;};
    std::vector<std::string> GetProd(){return prod_;};
    std::vector<int> GetAER(){return aer_;};
    std::vector<std::string> GetAM(){return am_;};
    std::vector<int> GetTiein(){return tie_in_;};
    std::vector<int> GetMinInvest(){return min_invest_;};
    std::vector<int> GetMaxInvest(){return max_invest_;};
    std::vector<int> GetAvail(){return avail_;};
    std::vector<int> GetMat(){return mat_;};
    void ChangeCap(int CashFlow, int Client);


};

int max_val(std::vector<int>);
std::vector<int> feed_reader(Algorithm &, int, std::vector<int>);
std::vector<int> feed_remov(Algorithm &,int, std::vector<int>);
std::vector<int> client_remov(Algorithm &,int, std::vector<int>);
bool date_safe(Algorithm &,int,int,Date &);
std::vector<int> feasible_prod(Algorithm &,int,std::vector<int>,const std::vector<int> &);
bool cap_safe(Algorithm &all_data,int client,int prod);
std::vector<int> sort_indexes(const std::vector<int> &);
#endif //PROJECT2_ALGORITHM_HPP
