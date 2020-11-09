//
// 1540006
//

#include "Algorithm.hpp"
#include "Date.hpp"
#include <algorithm>
#include <iostream>
#include <vector>
#include "StoreInput.hpp"
#include "Strategy.hpp"
#include <ctime>
#include <cmath>
#include <numeric>

//finds and outputs the max value of a vector
int max_val(std::vector<int> vec){
    int max = *max_element(vec.begin(), vec.end());
    return max;
}


// function to read through Feed.dat and moving any available products to the available container
std::vector<int> feed_reader(Algorithm &all_data,int curr_date,std::vector<int> vecs){

    int sz = all_data.GetAvail().size();

    for(int i{0};i<sz;i++){
        int temp{0};
        temp = all_data.GetAvail().at(i);
        if(temp == curr_date){
            vecs.push_back(i); // vector containing position of available products
            return vecs;
        }
    }
    return vecs;
}

// function to remove products available once maturity is reached

std::vector<int> feed_remov(Algorithm &all_data,int curr_date,std::vector<int> vecs){
    int sz_vecs = vecs.size(); //size of the available products vector
    for(int i{0};i<sz_vecs;i++){
        sz_vecs = vecs.size();
        int temp_el = vecs.at(i); //the position of the product being assessed
        int temp_dat = all_data.GetMat().at(temp_el); // the maturity of the product being assessed

        if(curr_date==temp_dat){
            vecs.erase(vecs.begin()+i);
            return vecs;
        }
    }
    return vecs;
}

// function to remove clients when they reach retirement

std::vector<int> client_remov(Algorithm &all_data,int curr_date, std::vector<int> vecc){
    //vecc = vector containing the positions of the clients that haven't retired - positions relating to ClientFeed.Clients
    int sz{0};
    sz = vecc.size(); //size of available client vector
    for(int i{0}; i<sz;i++){
        int temp_el = vecc.at(i); //position of client currently being assessed
        int temp_dat = all_data.GetRet().at(temp_el); //the retirement date of the client currently being assessed
        if(curr_date==temp_dat){
            vecc.erase(vecc.begin()+i);
        }
        sz = vecc.size();
    }
    return vecc;
}

//Checks enough time for client to invest in product

bool date_safe(Algorithm &all_data,int client,int prod, Date &dateobj){

    int c_ret = (all_data.GetMat()).at(client);
    int p_mat = all_data.GetMat().at(prod);
    int tie_in = all_data.GetTiein().at(prod);

    int end_tie_date = CalcDate(dateobj,tie_in);


    bool safe{false};
    if((c_ret>end_tie_date&& c_ret>p_mat)||(c_ret<tie_in&&c_ret>p_mat)){
        safe = true;
    }
    return safe;
}


//function to check whether the client has enough capital to invest in the product
bool cap_safe(Algorithm &all_data,int client,int prod){
    int clinet_cap = all_data.GetCap().at(client);
    int prod_min = all_data.GetMinInvest().at(prod);
    bool safe{false};
    if(clinet_cap>=prod_min){
        safe = true;
    }
    return safe;
}
//function to change the capital of the client in the objects database
void Algorithm::ChangeCap(int CashFlow, int Client) {
    cap_.at(Client) = cap_.at(Client)+CashFlow; //cap in pence
}
//function to sort a vector and returns the index postions
std::vector<int> sort_indexes(const std::vector<int> &v) {


    std::vector<int> indx(v.size());
    std::iota(indx.begin(), indx.end(), 0);

    std::stable_sort(indx.begin(), indx.end(),
                     [&v](int i1, size_t i2) {return v[i1] < v[i2];});

    return indx;
}


