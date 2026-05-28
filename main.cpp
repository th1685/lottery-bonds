#include <iostream>
#include <fstream>
#include <iomanip>
#include <numeric>
#include <ostream>
#include <random>
#include <vector>
#include <algorithm>
#include <ctime>
#include <sstream>
#include <windows.h>


bool checkPrizes(std::vector<std::vector<double>>& prizes) {
    double sum = 0;


    for (int i = 0; i < prizes[0].size(); i++) {
        sum += prizes[0][i] * prizes[1][i];
    }


    return (sum == 1.0);
}


double nominalFromEffective(double effective, int convertible) {
    return (pow((1 + effective), 1.0/(double)convertible) - 1);
}


double mean(std::vector<double> &v) {
    double sum = 0.0;
    for (int i = 0; i < v.size(); i++) {
        sum += v[i];
    }


    return sum / (double)v.size();
}


double median(std::vector<double> &v) {
    size_t n = v.size() / 2;
    nth_element(v.begin(), v.begin()+n, v.end());
    return v[n];
}


double standardDeviation(const std::vector<double>& values) {
    if (values.empty())
        throw std::invalid_argument("Vector must not be empty");


    double mean = std::accumulate(values.begin(), values.end(), 0.0) / values.size();


    double variance = 0.0;
    for (double v : values)
    {
        variance += (v - mean) * (v - mean);
    }


    variance /= values.size(); // Population standard deviation
    // Use (values.size() - 1) for sample standard deviation


    return std::sqrt(variance);
}


double toPennies(double x) {
    return std::round(x * 100.00) / 100;
}


void printAccounts (const std::vector<double>& accounts, int rows) {
    std::cout << "balances: " << std::endl;
    for (int i = 0; i < accounts.size() / rows; i++) {
        for (int j = 0; j < rows; j++) {
            std::cout << std::setw(8) << /*toPennies*/accounts[i * rows + j] << ", ";
        }
        std::cout << std::endl;
    }
}


void balancesToFile(std::ofstream& File, const std::vector<double>& accounts) {
    for (int i = 0; i < accounts.size(); i++) {
        File << accounts[i] << ", ";
    }
    File << std::endl;
}


std::string getTime() {
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);


    std::ostringstream oss;
    oss << std::put_time(localTime, u8"%Y-%m-%d %H:%M:%S");


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
    SetConsoleOutputCP(CP_UTF8);
    /*+CDML--------------------------------------------+*/
    if (argc != 6) {std::cout << "wrong number of arguments: "
                              << std::endl << "  -maturity -effective rate -convertible -max deposit -max accounts" << std::endl;
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
    std::string filename = "balances" + getTime() + ".csv";
    std::ofstream seriesBalances(filename);
    seriesBalances << "period,balances" << std::endl;
    
    double prize_fund = 0.0; //prize fund
    double maturity = std::stod(argv[1]); //years
    int convertible = std::stod(argv[2]);
    double effective_rate = std::stod(argv[3]); //annual interest rate
    double nominal_rate = nominalFromEffective(effective_rate, convertible);
    int max_deposit = std::stoi(argv[4]);
    int max_account_quantity = std::stoi(argv[5]);


    /*double maturity, effective_rate;
    int convertible, max_deposit, max_account_quantity;
    takeInputs(maturity, convertible, effective_rate, max_deposit, max_account_quantity);


    double nominal_rate = nominalFromEffective(effective_rate, convertible);*/


    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> A(1, max_account_quantity);


    std::vector<double> accounts(A(rng), 0.0);
    randomiseAccounts(accounts, max_deposit, rng);
    
    /*std::vector<double> accounts = {  //each account buys a certain number of contracts until the volume is fulfilled
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1, //60 x £1 accounts
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
         2,  2,  2,  2,  2,  2,  2,  2,  2,  2, //25 x £2 accounts
         2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
         2,  2,  2,  2,  2,
         5,  5,  5,  5,  5,  5,  5,  5,  5,  5, //10 x £5 accounts
        10, 10, 10, 10, 10, 10, 10, 10, 10, 10, // 2 x £10 accounts
        10, 10, 10, 10
    };*/


    std::vector<double> initial_balances = accounts;
    std::vector<double> increases(accounts.size(), 0.0);


    double X = accumulate(accounts.begin(), accounts.end(), 0.0);
    double simple_accrual = X * pow((1.0 + effective_rate), maturity);


    std::vector<std::vector<double>> prizes = {
        {1.0, 2.0, 4.0, 5.0}, //quantity 
        {0.25, 0.125, 0.1, 0.02} //fraction of prize fund
    };


    /**/
    
    if (!checkPrizes(prizes)) { //check prize payout
        std::cout << "prizes are initialised incorrectly." << std::endl;
        return 1;
    } std::cout << "prizes are initialised correctly." << std::endl;


    /*+RATES-------------------------------------------+*/
    std::cout << "annual effective rate: " << effective_rate * 100 << "\%"
              << std::endl << "  convertible " << convertible
              << " times per year" << ((maturity == 1) ? "" : " for " + std::to_string((int)(maturity)) + " years")
              << std::endl;
    
    std::cout << "nominal rate: ~" << 100.0 * nominal_rate << "\%" << std::endl;


    std::cout << std::endl << "total draws: " << (int)(accumulate(prizes[0].begin(), prizes[0].end(), 0.0) * maturity * convertible) << std::endl;
    std::cout << "accounts quantity: " << accounts.size() << std::endl;
    std::cout << std::endl << "initial book volume: £" << /*toPennies*/accumulate(initial_balances.begin(), initial_balances.end(), 0.0) << std::endl;
    /*std::cout << std::endl << "starting "; printAccounts(accounts, 10); std::cout << std::endl;*/


    /*+MAIN--------------------------------------------+*/
    for (int period = 1; period <= convertible * maturity; period++) {
        seriesBalances << period << ", ";
        prize_fund = X * nominal_rate;


        for (int i = 0; i < prizes[0].size(); i++) {
            for (int k = 1; k <= prizes[0][i]; k++) {
                /*Y ~ U(0, current volume)
                random number Y in [0, current volume) is chosen uniformly
                suppose [0, X) is subdivided into intervals [0, a_1), [a_1, a_2), etc,
                where a_{i+1}-a_i = account[i]. Then identify which interval contains Y.*/
                std::uniform_real_distribution<double> U(0, X);
                double Y = U(rng), sum = 0.0;
                int j = 0;
                while (sum < Y) {
                    sum += accounts[j];
                    j++;
                }
                accounts[j - 1] += prize_fund * prizes[1][i];
            }
        }


        balancesToFile(seriesBalances, accounts);
        prize_fund = 0.0;
        X = accumulate(accounts.begin(), accounts.end(), 0.0);
    }
    
    seriesBalances.close();


    /*+STATS-------------------------------------------+*/
    for (int i = 0; i < accounts.size(); i++) {
        increases[i] = (accounts[i] / initial_balances[i] - 1.00) * 100.00;
    }


    double book_increase = (accumulate(accounts.begin(), accounts.end(), 0.0) / accumulate(initial_balances.begin(), initial_balances.end(), 0.0) - 1.00) * 100.00;


    std::cout << u8"final book volume: £" << /*toPennies*/X<< std::endl;
    std::cout << ((toPennies(simple_accrual) == toPennies(X)) ? "  matches" : "  doesn't match") << " simple interest: £" << /*toPennies*/simple_accrual << std::endl;
    
    std::cout << std::endl << "final ";
    printAccounts(accounts, 10);


    std::cout << std::endl;
    std::cout << "median increase: " << /*toPennies*/median(increases) << "\% (book: " << /*toPennies*/book_increase << "\%)" << std::endl;
    std::cout << "average increase: " << /*toPennies*/mean(increases) << "\%" << std::endl;
    std::cout << "standard deviation: " << /*toPennies*/standardDeviation(increases) << "\%" << std::endl;
    
    std::cout << std::endl << "written to " << filename << std::endl;


    return 0;
}