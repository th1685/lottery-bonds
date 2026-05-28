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


double nominalFromEffective(double effective, int convertible) {
    return (pow((1 + effective), 1.0/static_cast<double>(convertible)) - 1);
}


double mean(const std::vector<double> &v) {
    return std::accumulate(v.begin(), v.end(), 0.0) / static_cast<double>(v.size());
}


double median(std::vector<double> v) {
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
    std::cout << "balances:\n";
    size_t col = 0;
    for (const double& val : accounts) {
        std::cout << std::setw(8) << val << ", ";
        if (++col % static_cast<size_t>(rows) == 0) {std::cout << "\n";}
    }
}


void balancesToFile(std::ofstream& File, const std::vector<double>& accounts) {
    for (const double& x : accounts) {
        File << x << ", ";
    }
    File << "\n"; //performance
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
    int convertible = std::stoi(argv[2]);
    double effective_rate = std::stod(argv[3]); //annual interest rate
    double nominal_rate = nominalFromEffective(effective_rate, convertible);
    int max_deposit = std::stoi(argv[4]);
    int max_account_quantity = std::stoi(argv[5]);


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
    double initial_vol = accumulate(initial_balances.begin(), initial_balances.end(), 0.0);
    double simple_accrual = X * pow((1.0 + effective_rate), maturity);


    std::vector<std::vector<double>> prizes = {
        {1.0, 2.0, 4.0, 5.0}, //quantity 
        {0.25, 0.125, 0.1, 0.02} //fraction of prize fund
    };

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


    std::cout << std::endl << "total draws: " << static_cast<int>(accumulate(prizes[0].begin(), prizes[0].end(), 0.0) * maturity * convertible) << std::endl;
    std::cout << "accounts quantity: " << accounts.size() << std::endl;
    std::cout << std::endl << "initial book volume: £" << initial_vol << std::endl;


    /*+MAIN--------------------------------------------+*/
    for (int period = 1; period <= convertible * maturity; period++) {
        seriesBalances << period << ", ";
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


        balancesToFile(seriesBalances, accounts);
        X += prize_fund; //checkPrizes guarantees the prize fund is emptied
    }
    
    seriesBalances.close();


    /*+STATS-------------------------------------------+*/
    for (int i = 0; i < accounts.size(); i++) {
        increases[i] = (accounts[i] / initial_balances[i] - 1.00) * 100.00;
    }


    double book_increase = (X / initial_vol - 1.00) * 100.00;


    std::cout << u8"final book volume: £" << X << std::endl;
    std::cout << ((toPennies(simple_accrual) == toPennies(X)) ? "  matches" : "  doesn't match") << " simple interest: £" << simple_accrual << std::endl;
    
    std::cout << std::endl << "final ";
    printAccounts(accounts, 10);


    std::cout << std::endl;
    std::cout << "median increase: " << median(increases) << "\%" << std::endl;
    std::cout << "average increase: " << mean(increases) << "\% (book: " << book_increase << "\%)" << std::endl;
    std::cout << "standard deviation: " << standardDeviation(increases) << "\%" << std::endl;
    
    std::cout << std::endl << "written to " << filename << std::endl;


    return 0;
}