//
// 1540006
//

#include "StoreInput.hpp"
#include <iostream>
#include <vector>
#include <InputReader.hpp>
#include <memory>
#include "FixedWidthInputReader.hpp" // For templated factory
#include <string>
#include <queue>

//functions to print vectors
void print(std::vector<int> veci){
    int sz = veci.size();
    for(int i{0}; i<sz; i++){
        std::cout << veci[i] << std::endl;
    }
}
void print(std::vector<std::string> veci){
    int sz = veci.size();
    for(int i{0}; i<sz; i++){
        std::cout << veci[i] << std::endl;
    }
}
//Function to check whether there is a problem with the input
int input_checker(const std::string filename){
    std::unique_ptr<InputReader> reader;
    try {
        reader = InputReaderFactory(filename);
    } catch (const std::exception & e) {
        std::cerr << "A problem occurred opening " << filename << ": " << e.what() << std::endl;
        return 1; // Abort with error status
    }
    return 0;
}

int input_checker_feed(const std::string filename){
    std::unique_ptr<InputReader> reader;
    try {
        // For CSV or TSV (uses file extension to choose)
        //reader = InputReaderFactory(filename);
        // For fixed width fields - field widths are template arguments
        reader = InputReaderFactory<4,8,5,1,2,6,6,8,8>(filename);
    } catch (const std::exception & e) {
        std::cerr << "A problem occurred opening " << filename << ": " << e.what() << std::endl;
        return 1; // Abort with error status
    }
    return 0;
}
//Reads in the client data, stores the values in a struct
ClientData readin(const std::string filename, int n) {
    ClientData ClientFeed{{},{},{}};

    std::unique_ptr<InputReader> reader;

    reader = InputReaderFactory(filename);


    int first{0};

    while(first<n){
        //to remove the headers
        if(first == 0){
            reader->GetNextField();
            reader->GetNextField();
            reader->GetNextField();
        }

        for(int i{0}; i<3; i++){
            if(i == 0) {
                ClientFeed.Clients.push_back(reader->GetNextField());
            }else if(i == 1){
                ClientFeed.Cap.push_back(reader->GetNextField());
                } else{
                ClientFeed.Ret.push_back(reader->GetNextField());
            }

        }
        first++;
    }
    return ClientFeed;
}
//function to read in the product feed and store that in a struct
ProdData readin_feed(std::string filename, int n){



    ProdData ProdFeed{{},{},{},{},{},{},{},{},{}};

    std::unique_ptr<InputReader> reader;

    reader = InputReaderFactory<4,8,5,1,2,6,6,8,8>(filename);

    int count{0};
    while(count<n){

        for(int i{0};i<9;i++){
            if(i == 0){
                ProdFeed.Comp.push_back(reader->GetNextField());
            }else if(i ==1){
                ProdFeed.Prod.push_back(reader->GetNextField());
            }else if(i ==2){
                ProdFeed.AER.push_back(reader->GetNextField());
            }else if(i ==3){
                ProdFeed.AM.push_back(reader->GetNextField());
            }else if(i ==4){
                ProdFeed.tie_in.push_back(reader->GetNextField());
            }else if (i ==5){
                ProdFeed.min_invest.push_back(reader->GetNextField());
            }else if(i==6){
                ProdFeed.max_invest.push_back(reader->GetNextField());
            }else if (i==7){
                ProdFeed.Avail.push_back(reader->GetNextField());
            }else{
                ProdFeed.Mat.push_back(reader->GetNextField());
            }
        }
        count++;
    }
    return ProdFeed;
}
//constructor for the input object
StoreInput::StoreInput(std::vector<std::string> Clients,std::vector<int> Cap, std::vector<int> Ret,
                       std::vector<std::string> Comp,std::vector<std::string> Prod,std::vector<int> AER,
                       std::vector<std::string> AM,std::vector<int> tie_in,std::vector<int> min_invest,
                       std::vector<int> max_invest, std::vector<int> Avail,std::vector<int> Mat)
                       : clients_{Clients},cap_{Cap},ret_{Ret},
                       comp_{Comp},prod_{Prod},aer_{AER},am_{AM}, tie_in_{tie_in},
                       min_invest_{min_invest}, max_invest_{max_invest}, avail_{Avail}, mat_{Mat}{}

