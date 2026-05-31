#include "file_io.hpp"

int generateGraph(const std::string& graphname, const std::vector<std::vector<double>>& accounts_timeseries) {
    /*suppress gnuplot messages*/
    auto f = matplot::figure(false);
    f->backend()->run_command("unset warnings");
    f->ioff();

    /*transpose  timeseries*/
    size_t rows = accounts_timeseries.size();
    size_t cols = accounts_timeseries[0].size();

    std::vector<std::vector<double>> transposed(cols, std::vector<double>(rows));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            transposed[j][i] = accounts_timeseries[i][j];
        }
    }

    matplot::title("Balances per time");
    matplot::xlabel("Compounding period"); matplot::ylabel("Amount (£)");

    matplot::plot(transposed);

    matplot::save(graphname);
    std::cout << "chart saved to ./" + graphname << std::endl;
    
    return 0;
}


int balancesToFile(std::string& filename, const std::vector<std::vector<double>>& accounts_timeseries) {
    std::ofstream seriesBalances(filename);

    for (const std::vector<double>& x : accounts_timeseries) {
        for (const double& y : x) {
            seriesBalances << y << ", ";
        }
        seriesBalances << "\n"; //performance
    }

    seriesBalances.close();
    std::cout << "\nbalances data written to ./" << filename << std::endl;

    return 0;
}


bool is_empty(std::ifstream& pFile) {
    return pFile.tellg() == 0 && pFile.peek() == std::ifstream::traits_type::eof();
}

std::vector<double> load_1d(std::string filename) {
    std::ifstream filedata(filename);
    std::vector<double> data;

    if (!filedata.is_open()) { std::cout << "could not open " + filename << std::endl; return data; }
    if (is_empty(filedata)) { std::cout << filename + " empty" << std::endl; return data; }
    
    double file_read_account_value;
    int file_read_account_volume;
    while (filedata >> file_read_account_value >> file_read_account_volume) {
        data.insert(data.end(), file_read_account_volume, file_read_account_value);
    }
    filedata.close();

    return data;
}

std::vector<std::vector<double>> load_2d(std::string filename) {
    std::ifstream filedata(filename);
    std::vector<std::vector<double>> data(2);

    if (!filedata.is_open()) { std::cout << "could not open " + filename << std::endl; return data; }
    if (is_empty(filedata)) { std::cout << filename + " empty" << std::endl; return data; }
    
    double file_read_prizes_value, file_read_prizes_volume;
    while (filedata >> file_read_prizes_volume >> file_read_prizes_value) {
        data[0].push_back(file_read_prizes_volume);
        data[1].push_back(file_read_prizes_value);
    }
    filedata.close();

    return data;
}