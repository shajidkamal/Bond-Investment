//
// 1540006
//

#include "Strategy.hpp"
#include <iostream>
#include <string>
#include <memory>
#include <utility>
#include <vector>
#include "InputReader.hpp"
#include "FixedWidthInputReader.hpp" // For templated factory
#include "StoreInput.hpp"
#include "Date.hpp"
#include "Algorithm.hpp"
#include <numeric>
#include <ctime>
#include <cmath>
#include <map>
#include <fstream>

/*
 * Function that selects a potential product to invest in
 * Considers which available products has the highest AER
 * and then it checks to see whether the client has enough capital to meet the minimum investment requirement
 * and also whether they have enough time to invest before retirement.
 * If not, the program moves to the second highest and so on, and returns false if none are found
 */
bool Selector(Algorithm &all_data, int client,std::vector<int> &avail_prod,Date date_obj, int &pot_prod){
    bool ProdFound{false};
    int sz_prod = avail_prod.size();
    if(sz_prod==1){
        pot_prod = avail_prod.back();
    } else{
        std::vector<int> temp_aer;
        for(int i{0};i<sz_prod;i++){
            // loop through checking which product has the highest AER
            temp_aer.push_back(all_data.GetAER().at(avail_prod.at(i)));
        }
        std::vector<int> temp_indx_aer; //lowest to highest index ranked
        temp_indx_aer = sort_indexes(temp_aer);

        int final_prod;
        for(int j {0}; j<sz_prod; j++){
            final_prod = avail_prod.at(temp_indx_aer.at(j));
            if(date_safe(all_data,client,final_prod, date_obj) && cap_safe(all_data,client,final_prod)){
                pot_prod = final_prod;
                ProdFound = true;
                return ProdFound;
            }
        }
        return ProdFound;

    }
    return ProdFound;
}


/*
 * Investing function
 * Once a suitable product is found, this function pulls all relevant metrics and tries to invest
 * 50% of clients available capital. If this exceeds the min or max, the percentage is altered by 2%.
 * If this alteration happens 100 times, a failsafe is triggered and the minimum possible amount is invested
 * The transaction details are then copied into the Clients object
 * A struct containing the active investments is also output
 */

void Invest(int client,int prod, Algorithm &all_data, Date dateobj, std::vector<Strategy> &ClientObjects,
        ActiveInvestments &all_invests){
    double pct_invest{0.5};
    int client_cap = all_data.GetCap().at(client); //capital available in pence
    int tot_invst = std::floor(pct_invest*client_cap); //the amount to be invested in pence
    int min_invest = all_data.GetMinInvest().at(prod) *100; //min invest in pence
    int max_invest = all_data.GetMaxInvest().at(prod) *100; //max invest in pence


    int counting{0};
    while(!(tot_invst>=min_invest&&tot_invst<=max_invest)){ //while the amount invested isn't between the max and min invest

        if(counting>100){
            tot_invst = min_invest;
        } else if(tot_invst<min_invest){
            pct_invest = pct_invest+0.02;
            tot_invst = std::floor(pct_invest*client_cap); // Rounded to lowest int, in pence
        }else if(tot_invst>max_invest){
            pct_invest = pct_invest-0.02;
            tot_invst = std::floor(pct_invest*client_cap); //pence
        }
        counting++;
    }
    if(all_data.GetCap().at(client)- tot_invst<0){
        return;
    }

    all_invests.Clients.push_back(client);
    all_invests.Products.push_back(prod);
    all_invests.TieInEnd.push_back(CalcDate(dateobj,all_data.GetTiein().at(prod)));
    all_invests.ProdMat.push_back(all_data.GetMat().at(prod));
    all_invests.TransDate.push_back(dateobj.GetDate());
    all_invests.CashFlow.push_back(tot_invst); //in pence

    std::string Type{"Invest"};
    ClientObjects.at(client).Receipt(dateobj.GetDate(),all_data.GetComp().at(prod),all_data.GetProd().at(prod), Type, tot_invst);

    tot_invst = tot_invst*(-1);
    all_data.ChangeCap(tot_invst,client);


}

//function to calculate how much interest was accrued and getting everything back

void SellRedeem(Algorithm &all_data, Date dateobj, std::vector<Strategy> &ClientObjects,
        ActiveInvestments &all_invests, int active_indx){

    int client{all_invests.Clients.at(active_indx)};
    int prod{all_invests.Products.at(active_indx)};
    int trans_date{all_invests.TransDate.at(active_indx)};
    int tot_invst{all_invests.CashFlow.at(active_indx)};
    int prod_mat{all_invests.ProdMat.at(active_indx)};

    std::string AM = all_data.GetAM().at(prod);
    int curr_date = dateobj.GetDate();
    int cash_flow{0};

    if(AM == "A"){

        int time_elapsed = curr_date - trans_date;
        time_elapsed = std::floor(time_elapsed/10000); //number of years elapsed
        double aer = all_data.GetAER().at(prod);
        aer = aer/10000; // e.g. 520 -> 0.052%
        cash_flow = floor(tot_invst*pow((1+aer),time_elapsed)); //compund interest formulae

    }else if(AM == "M"){
        //calculating how many month anniversaries were completed
        std::vector<int> month_days;
        month_days = {31,28,31,30,31,30,31,31,30,31,30,31};
        std::vector<int> temp_date_vec;
        temp_date_vec = DecompDate(trans_date);
        Date transobj(temp_date_vec.at(0),temp_date_vec.at(1),temp_date_vec.at(2));
        int daysbetw = CalcDays(dateobj,transobj);
        std::vector<int> curr_date_vec{dateobj.Singles()};
        int monthindx = temp_date_vec.at(1) -1;
        int count{0};

        while(daysbetw>=0) {
            daysbetw = daysbetw - month_days.at(monthindx);

            if (monthindx == 12) {
                monthindx = 0;

            }
            count++;
        }

        if((temp_date_vec.at(2)==28 && curr_date_vec.at(1) ==2 )||(temp_date_vec.at(2)==30 && month_days.at(curr_date_vec.at(1) -1)) ){
            count++;
        }

        int months_completed{count};
        //calc interest, converting from aer to monthly
        double aer = all_data.GetAER().at(prod);
        aer = aer/10000; // e.g. 520 -> 0.052%
        double intrate = pow(1+aer,1.0/12);
        intrate = intrate-1;
        cash_flow = floor(tot_invst*pow((1+intrate),months_completed));

    }

    std::string Type;
    if(curr_date == prod_mat){
        Type = "Redeem";
    }else{
        Type = "Sell";
    }
    //change the available cap for client
    all_data.ChangeCap(cash_flow,client);

    ClientObjects.at(client).Receipt(dateobj.GetDate(),all_data.GetComp().at(prod), all_data.GetProd().at(prod),
                                     Type, cash_flow);


}




/*
 *function that decides next steps once tie in has expired
 */

bool TieInHit(int client, int prod, ActiveInvestments &all_invests, Date dateobj, Algorithm &all_data,std::vector<int> &avail_prod,
        int active_indx, std::vector<Strategy> &ClientObjects){
    //run the selector function, get a product, compare aer to current one
    //if new aer is higher, sell and recoup interest.
    //then immediately sell and invest in the new product
    //else, keep a hold of the current product - do nothing and change nothing.

    int pot_prod{5000};
    if(Selector(all_data,client,avail_prod,dateobj,pot_prod)){
        int pot_aer = all_data.GetAER().at(pot_prod);
        int curr_aer = all_data.GetAER().at(prod);
        if(pot_aer>curr_aer){
            SellRedeem(all_data,dateobj, ClientObjects, all_invests,active_indx);
            Invest(client, pot_prod,all_data,dateobj,ClientObjects,all_invests);
            return true;
        }
        }else{
            return false;
    }
    return false;
}


//Constructor for the client objects
Strategy::Strategy(std::string ClientName):
                    client_{std::move(ClientName)}, trans_date_{},prod_comp_{},prod_name_{},trans_type_{},cash_flow_{}{}

//to record transactions
void Strategy::Receipt(int TransDate, const std::string& Comp, const std::string& ProdName, const std::string& TransType, int CashFlow) {
    trans_date_.emplace_back(TransDate);
    prod_comp_.emplace_back(Comp);
    prod_name_.emplace_back(ProdName);
    trans_type_.emplace_back(TransType);
    cash_flow_.emplace_back(CashFlow);
}

//Checks if maturity is hit
void EndChecker(ActiveInvestments &all_invests, Date dateobj, Algorithm &all_data, std::vector<Strategy> &ClientObjects){
    int temp_sz = all_invests.Clients.size();
    for(int i{temp_sz-1};i>0;i--){
        int curr_date = dateobj.GetDate();
        int active_indx{i};
        int mat_date = all_invests.ProdMat.at(i);
        if(curr_date == mat_date){

            //sets values to be deleted to an arbitrary number
            SellRedeem(all_data,dateobj,ClientObjects,all_invests,active_indx);
            all_invests.Clients[active_indx] = 500000;
            all_invests.Products[active_indx]= 500000;
            all_invests.TransDate[active_indx]= 500000;
            all_invests.CashFlow[active_indx]= 500000;
            all_invests.ProdMat[active_indx]= 500000;
            all_invests.TieInEnd[active_indx]= 500000;

        }

    }
    //values with said arbitrary numbers are then removed, removing any active investment that has expired
    all_invests.Clients.erase(std::remove(all_invests.Clients.begin(),all_invests.Clients.end(),500000),all_invests.Clients.end());
    all_invests.Products.erase(std::remove(all_invests.Products.begin(),all_invests.Products.end(),500000),all_invests.Products.end());
    all_invests.TransDate.erase(std::remove(all_invests.TransDate.begin(),all_invests.TransDate.end(),500000),all_invests.TransDate.end());
    all_invests.CashFlow.erase(std::remove(all_invests.CashFlow.begin(),all_invests.CashFlow.end(),500000),all_invests.CashFlow.end());
    all_invests.ProdMat.erase(std::remove(all_invests.ProdMat.begin(),all_invests.ProdMat.end(),500000),all_invests.ProdMat.end());
    all_invests.TieInEnd.erase(std::remove(all_invests.TieInEnd.begin(),all_invests.TieInEnd.end(),500000),all_invests.TieInEnd.end());
}

//function to check whether tie in has expired
void TieChecker(ActiveInvestments &all_invests, Date dateobj, Algorithm &all_data, std::vector<Strategy> &ClientObjects, std::vector<int> &avail_prod){
    std::vector<int> to_delete;
    int temp_sz = all_invests.Clients.size();
    for(int i{temp_sz-1};i>0;i--){
        int curr_date = dateobj.GetDate();
        int client = all_invests.Clients.at(i);
        int active_indx{i};
        int prod = all_invests.Products.at(i);
        int tie_in_end = all_invests.TieInEnd.at(i);
        if(curr_date == tie_in_end){
            if(TieInHit(client,prod,all_invests,dateobj,all_data,avail_prod,active_indx,ClientObjects)){
                SellRedeem(all_data,dateobj,ClientObjects,all_invests,active_indx);
                all_invests.Clients[active_indx] = 500000;
                all_invests.Products[active_indx]= 500000;
                all_invests.TransDate[active_indx]= 500000;
                all_invests.CashFlow[active_indx]= 500000;
                all_invests.ProdMat[active_indx]= 500000;
                all_invests.TieInEnd[active_indx]= 500000;
            }

        }

    }
    all_invests.Clients.erase(std::remove(all_invests.Clients.begin(),all_invests.Clients.end(),500000),all_invests.Clients.end());
    all_invests.Products.erase(std::remove(all_invests.Products.begin(),all_invests.Products.end(),500000),all_invests.Products.end());
    all_invests.TransDate.erase(std::remove(all_invests.TransDate.begin(),all_invests.TransDate.end(),500000),all_invests.TransDate.end());
    all_invests.CashFlow.erase(std::remove(all_invests.CashFlow.begin(),all_invests.CashFlow.end(),500000),all_invests.CashFlow.end());
    all_invests.ProdMat.erase(std::remove(all_invests.ProdMat.begin(),all_invests.ProdMat.end(),500000),all_invests.ProdMat.end());
    all_invests.TieInEnd.erase(std::remove(all_invests.TieInEnd.begin(),all_invests.TieInEnd.end(),500000),all_invests.TieInEnd.end());
}

//function to check whether client has hit retirement
void RetChecker(ActiveInvestments &all_invests, Date dateobj, Algorithm &all_data, std::vector<Strategy> &ClientObjects, std::vector<int> &avail_prod){
        int temp_sz = all_invests.Clients.size();
        for(int i{0};i<temp_sz;i++){
            int curr_date = dateobj.GetDate();
            int client = all_invests.Clients.at(i);
            int ret_date = all_data.GetRet().at(client);
            int active_indx{i};
            int prod = all_invests.Products.at(i);
            int tie_in_end = all_invests.TieInEnd.at(i);
            if(curr_date == ret_date && curr_date>=tie_in_end){
                    SellRedeem(all_data,dateobj,ClientObjects,all_invests,active_indx);
                    all_invests.Clients[active_indx] = 500000;
                    all_invests.Products[active_indx]= 500000;
                    all_invests.TransDate[active_indx]= 500000;
                    all_invests.CashFlow[active_indx]= 500000;
                    all_invests.ProdMat[active_indx]= 500000;
                    all_invests.TieInEnd[active_indx]= 500000;

            }

        }
        all_invests.Clients.erase(std::remove(all_invests.Clients.begin(),all_invests.Clients.end(),500000),all_invests.Clients.end());
        all_invests.Products.erase(std::remove(all_invests.Products.begin(),all_invests.Products.end(),500000),all_invests.Products.end());
        all_invests.TransDate.erase(std::remove(all_invests.TransDate.begin(),all_invests.TransDate.end(),500000),all_invests.TransDate.end());
        all_invests.CashFlow.erase(std::remove(all_invests.CashFlow.begin(),all_invests.CashFlow.end(),500000),all_invests.CashFlow.end());
        all_invests.ProdMat.erase(std::remove(all_invests.ProdMat.begin(),all_invests.ProdMat.end(),500000),all_invests.ProdMat.end());
        all_invests.TieInEnd.erase(std::remove(all_invests.TieInEnd.begin(),all_invests.TieInEnd.end(),500000),all_invests.TieInEnd.end());
    }

//function to output a series of client csv files
void CSVWriter(std::vector<Strategy> ClientObjects){
    int sz_obj = ClientObjects.size();
    for(int i{0}; i<sz_obj;i++){
        std::string outfilename{ClientObjects.at(i).GetClient()};

        std::ofstream file;
        file.open(outfilename);

        int sz_rec = ClientObjects.at(i).GetCash().size();
        for(int j{0};j<sz_rec;j++){
            file << ClientObjects.at(i).GetTrans().at(j) << ","
            << ClientObjects.at(i).GetProdComp().at(j) << ","
            << ClientObjects.at(i).GetProdName().at(j) << ","
            << ClientObjects.at(i).GetType().at(j) <<","
            << ClientObjects.at(i).GetCash().at(j) << std::endl;

        }
        file.close();


    }
}