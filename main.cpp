// C++ for Finance: Assignment 2: 1540006
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
#include "Strategy.hpp"
#include <ctime>
#include <map>
#include <fstream>
// This program will implement an algorithm to invest clients capital in bond like products.
// Program runs for approx. 5 mins on sample data
// Note: .idea folder could not be deleted since it isnt visible on mac

int main() {

    //Input to indicate how many clients and products will be evaluated
    int num_client{0};
    int num_prod{0};
    std::cout<< "Please Enter the Number of Clients inside the CSV/TSV file:"<< std::endl;
    std::cin>> num_client;

    std::cout<< "Please Enter the Number of Products inside the file:"<< std::endl;
    std::cin>> num_prod;
    //Initialising empty structs to temporarily store input data before being transferred into an object
    ClientData ClientFeed{{},{},{}}; //structs
    ProdData ProdFeed{{},{},{},{},{},{},{},{},{}};

    const std::string filename {"../People.csv"}; // Input filename
    const std::string filename1 {"../Feed.dat"}; // Input filename

    //Checking if the input is good, then it imports in all the data
    if(input_checker(filename) == 0){
        ClientFeed = readin(filename,num_client);
    }else{
        return input_checker(filename);
    }

    if(input_checker_feed(filename1) == 0){
        ProdFeed = readin_feed(filename1,num_prod);
    }else{
        return input_checker_feed(filename1);
    }

    //Constructing the object storing all the data
    Algorithm all_data(ClientFeed.Clients,ClientFeed.Cap,ClientFeed.Ret,
            ProdFeed.Comp,ProdFeed.Prod,ProdFeed.AER,ProdFeed.AM,ProdFeed.tie_in,ProdFeed.min_invest,
            ProdFeed.max_invest, ProdFeed.Avail,ProdFeed.Mat); //Data Object

    //Constructing the date object to 5/4/20

    Date date_1(2020,4,5); //Date Object

    //Setting the current date and the end date for our program - when the last person retires

    int curr_date{date_1.GetDate()};
    int end_date{max_val(ClientFeed.Ret)};

    //Creating a vector, avail_clients, to store the clients that are currently available
    //They are stored as "int" referring to the index number of the container inside the all_data object
    //The vector will constantly be checked to see whether any clients have reached retirement, and will subsequently be removed


    std::vector<int> avail_clients(num_client);
    std::iota(std::begin(avail_clients),std::end(avail_clients),0);

    //Creating a vector to store products once they become available
    std::vector<int> avail_prod(0);

    //creating receipt objects for each client, all transaction data will be stored here
    //Structure of the vector is identical to all_data.GetClient i.e. the indexes match

    std::vector<Strategy> ClientObjects;
    for(int i{0}; i<num_client;i++){
        std::string temp_client = all_data.GetClient().at(i);
        temp_client = temp_client + ".csv";
        ClientObjects.emplace_back(temp_client);
    }

    //Initialising an empty struct to store all active investments and relevant metrics
    ActiveInvestments all_invests{};

    //Main while loop: it will increment the day by 1 until the last person retires

    while(curr_date!=end_date){

        avail_prod = feed_reader(all_data,curr_date,avail_prod); //adding available products
        avail_prod = feed_remov(all_data,curr_date,avail_prod); //removing products that reached maturity
        avail_clients = client_remov(all_data,curr_date,avail_clients); //removing any clients that have reached retirement

        int sz_prod = avail_prod.size();
        int sz_cli = avail_clients.size();

        //Checks to see whether a potential product to invest exists, if it does it will invest in it.

        for(int i{0}; i<sz_cli;i++){
            int client = i;
            int pot_prod{50000};
            if(Selector(all_data,client,avail_prod,date_1,pot_prod)){
                Invest(client,pot_prod,all_data,date_1,ClientObjects,all_invests);
            }
        }

        //Checks whether any active investments have reached their tie in date
        TieChecker(all_invests,date_1,all_data,ClientObjects,avail_prod);
        //Checks whether any active investments have reached their maturity
        EndChecker(all_invests,date_1,all_data,ClientObjects);
        //Checks clients reaching retirement have any outstanding investments
        RetChecker(all_invests,date_1,all_data,ClientObjects,avail_prod);



        date_1.NextDate();
        curr_date = date_1.GetDate();
    }

    //Outputs CSV files for each client
    CSVWriter(ClientObjects);

    return 0;
}


