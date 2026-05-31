#include <cstddef>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <numeric>
#include <ostream>
#include <random>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <sstream>
#include <matplot/matplot.h>

#include "headers/numerical.hpp"
#include "headers/file_io.hpp"

#ifdef _WIN32
  #include <windows.h>
#endif

bool checkPrizes(const std::vector<std::vector<double>>& prizes) {
    double sum = std::inner_product(
        prizes[0].begin(), prizes[0].end(),
        prizes[1].begin(), 0.0
    );
    return std::fabs(sum - 1.0) < 1e-9;
}


void printAccounts (const std::vector<double>& accounts, int rows) {
    std::cout << "balances:\n";
    size_t col = 0;
    for (const double& val : accounts) {
        std::cout << std::setw(8) << val << ", ";
        if (++col % static_cast<size_t>(rows) == 0) {std::cout << "\n";}
    }
    std::cout << "\n";
}


std::string getTime() {
    std::ostringstream oss;
    std::time_t now = std::time(nullptr);
    std::tm localTime{};
    #ifdef _WIN32
        localtime_s(&localTime, &now);
    #else
        localtime_r(&now, &localTime);
    #endif
    oss << std::put_time(&localTime, u8"%Y-%m-%d T%H%M%S");

    std::string dateTime = oss.str();
    
    return dateTime;
}


void randomiseAccounts (std::vector<double>& accounts, int max_deposit, std::mt19937& rng) {
    std::uniform_int_distribution<int> A(1, max_deposit);

    for (double& account : accounts) {
        account += A(rng);
    }
}


void takeInputs(double& maturity, int& convertible, double& effective_rate, int& max_deposit, int& max_account_quantity) {
    std::cout << "maturity (years) : ";
    std::cin >> maturity;

    std::cout << "effective annual rate (%): ";
    std::cin >> effective_rate; effective_rate /= 100.00;

    std::cout << "compounding frequency (times per year): ";
    std::cin >> convertible;

    std::cout << "max deposit: ";
    std::cin >> max_deposit;

    std::cout << "max accounts: ";
    std::cin >> max_account_quantity;
}


int main(int argc, char* argv[]) {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif

    /*+CDML--------------------------------------------+*/
    if (argc != 6) {std::cout << "wrong number of arguments: "
                              << std::endl << "  -maturity -convertible -effective rate -max deposit -max accounts" << std::endl;
                    return 1;}
    else if (
        std::stod(argv[1]) < 0.0 ||
        std::stod(argv[2]) < 0.0 ||
        std::stod(argv[3]) < 0.0 ||
        std::stod(argv[4]) < 0.0 ||
        std::stod(argv[5]) < 0.0
        ) {
        std::cout << "enter positive quantities" << std::endl;

        return 1;
    }


    /*+INIT--------------------------------------------+*/
    std::cout << std::fixed << std::setprecision(2);
    std::string timestamp = getTime();
    std::string output_folder = "data-output"; output_folder += "/";
    std::string filename = output_folder + "balances" + timestamp + ".csv";
    std::string graphname = output_folder + "graph" + timestamp + ".jpg";
    
    double prize_fund = 0.0; //prize fund
    double maturity = std::stod(argv[1]); //years
    int convertible = std::stoi(argv[2]);
    double effective_rate = std::stod(argv[3]); //annual interest rate
    double nominal_rate = nominalFromEffective(effective_rate, convertible);
    int max_deposit = std::stoi(argv[4]);
    int max_account_quantity = std::stoi(argv[5]);

    std::random_device dev;
    std::mt19937 rng(dev());
    /*std::uniform_int_distribution<int> A(1, max_account_quantity);*/
    /*std::vector<double> accounts(A(rng), 0.0);*/ //empty random size

    /*load file data*/
    std::vector<double> accounts = load_1d("config/accounts.txt");
    std::vector<std::vector<double>> prizes = load_2d("config/p-structure.txt");
    std::vector<double> increases(accounts.size(), 0.0);
    
    /*init & checks*/
    std::vector<std::vector<double>> accounts_timeseries; //empty series
    /*randomiseAccounts(accounts, max_deposit, rng);*/ //random start balances
    accounts_timeseries.push_back(accounts); //push initial

    double X = accumulate(accounts.begin(), accounts.end(), 0.0);
    double initial_vol = X;
    double simple_accrual = X * pow((1.0 + effective_rate), maturity);

    if (!checkPrizes(prizes)) { //check prize payout
        std::cout << "prizes are initialised incorrectly." << std::endl;
        return 1;
    } std::cout << "prizes are initialised correctly." << std::endl;


    /*+RATES-------------------------------------------+*/
    std::cout << "annual effective rate: " << effective_rate * 100 << "\%"
              << std::endl << "  convertible " << convertible
              << " times per year" << ((maturity == 1) ? "" : " for " + std::to_string(static_cast<int>(maturity)) + " years")
              << std::endl;
    
    std::cout << "nominal rate: ~" << 100.0 * nominal_rate << "\%" << std::endl;

    std::cout << std::endl << "total prize draws: " << static_cast<int>(accumulate(prizes[0].begin(), prizes[0].end(), 0.0) * maturity * convertible) << std::endl;
    std::cout << "accounts quantity: " << accounts.size() << std::endl;
    std::cout << std::endl << "initial book volume: £" << initial_vol << std::endl;


    /*+MAIN--------------------------------------------+*/
    for (int period = 1; period <= convertible * maturity; period++) {
        //seriesBalances << period << ", ";
        prize_fund = X * nominal_rate;

        std::uniform_real_distribution<double> U(0, X);
        for (int i = 0; i < prizes[0].size(); i++) {
            for (int k = 1; k <= static_cast<int>(prizes[0][i]); k++) {
                /*Y ~ U(0, current volume)
                random number Y in [0, current volume) is chosen uniformly
                suppose [0, X) is subdivided into intervals [0, a_1), [a_1, a_2), etc,
                where a_{i+1}-a_i = account[i]. Then identify which interval contains Y.*/
                double Y = U(rng), sum = 0.0;
                int j = 0;
                while (sum < Y) {
                    sum += accounts[j];
                    j++;
                }
                accounts[j - 1] += prize_fund * prizes[1][i];
            }
        }

        X += prize_fund; //checkPrizes guarantees the prize fund is emptied
        accounts_timeseries.push_back(accounts); //add current to series
    }

    /*+STATS-------------------------------------------+*/
    for (int i = 0; i < accounts.size(); i++) {
        increases[i] = (accounts[i] / accounts_timeseries[0][i] - 1.00) * 100.00;
    }

    double book_increase = (X / initial_vol - 1.00) * 100.00;

    std::cout << u8"final book volume: £" << X << std::endl;
    std::cout << ((toPennies(simple_accrual) == toPennies(X)) ? "  matches" : "  doesn't match") << " simple interest: £" << simple_accrual << std::endl;
    
    std::cout << std::endl << "final ";
    printAccounts(accounts, 10);

    std::cout << "median increase: " << median(increases) << "\%" << std::endl;
    std::cout << "average increase: " << mean(increases) << "\% (book: " << book_increase << "\%)" << std::endl;
    std::cout << "standard deviation: " << standardDeviation(increases) << "\%" << std::endl;

    if (balancesToFile(filename, accounts_timeseries)) { std::cout << "\nfailed to write balances data to file" << std::endl; }
    if (generateGraph(graphname, accounts_timeseries)) { std::cout << "\nfailed to plot graph" <<std::endl; }

    return 0;
}