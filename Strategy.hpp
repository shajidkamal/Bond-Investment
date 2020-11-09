//
// 1540006
//
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include "InputReader.hpp"
#include "FixedWidthInputReader.hpp" // For templated factory
#include "StoreInput.hpp"
#include "Date.hpp"
#include "Algorithm.hpp"
#include <numeric>
#include <ctime>
#include <map>
#ifndef PROJECT2_STRATEGY_HPP
#define PROJECT2_STRATEGY_HPP

struct ActiveInvestments{
    std::vector<int> Clients; //index referring to original client structure
    std::vector<int> Products; //index referring to original product structure
    std::vector<int> TieInEnd; //when the tie in period ends
    std::vector<int> ProdMat;
    std::vector<int> TransDate;
    std::vector<int> CashFlow; // needs to be in pence
};



class Strategy{
protected:
    std::string client_;
    std::vector<int> trans_date_;
    std::vector<std::string> prod_comp_;
    std::vector<std::string> prod_name_;
    std::vector<std::string> trans_type_;
    std::vector<int> cash_flow_;
public:
    explicit Strategy(std::string ClientName); //constructor
    void Receipt(int TransDate,const std::string& Comp, const std::string& ProdName, const std::string& TransType, int CashFlow);
    std::vector<int> GetCash(){return cash_flow_;};
    std::vector<std::string> GetType(){return trans_type_;};
    std::string GetClient(){return client_;};
    std::vector<std::string> GetProdComp(){return prod_comp_;};
    std::vector<std::string> GetProdName(){return prod_name_;};
    std::vector<int> GetTrans(){return trans_date_;};


};

bool Selector(Algorithm &all_data, int client,std::vector<int> &avail_prod,Date date_obj, int &pot_prod);
void Invest(int client,int prod, Algorithm &all_data, Date dateobj, std::vector<Strategy> &ClientObjects,
                         ActiveInvestments &all_invests);

bool TieInHit(int client, int prod, ActiveInvestments &all_invests, Date dateobj, Algorithm &all_data,std::vector<int> &avail_prod,
              int active_indx, std::vector<Strategy> &ClientObjects);
void SellRedeem(Algorithm &all_data, Date dateobj, std::vector<Strategy> &ClientObjects,
                ActiveInvestments &all_invests, int active_indx);
void CheckEnd(ActiveInvestments &all_invests, Date dateobj, std::vector<Strategy> &ClientObjects, Algorithm &all_data,
              std::vector<int> &avail_prod);
void EndChecker(ActiveInvestments &all_invests, Date dateobj, Algorithm &all_data, std::vector<Strategy> &ClientObjects);

void TieChecker(ActiveInvestments &all_invests, Date dateobj, Algorithm &all_data, std::vector<Strategy> &ClientObjects, std::vector<int> &avail_prod);
void RetChecker(ActiveInvestments &all_invests, Date dateobj, Algorithm &all_data, std::vector<Strategy> &ClientObjects, std::vector<int> &avail_prod);
void CSVWriter(std::vector<Strategy> ClientObjects);
#endif //PROJECT2_STRATEGY_HPP
