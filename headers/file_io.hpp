#ifndef FILEIO_H
#define FILEIO_H

#include <vector>
#include <matplot/matplot.h>

int generateGraph(const std::string& graphname, const std::vector<std::vector<double>>& accounts_timeseries);

int balancesToFile(std::string& filename, const std::vector<std::vector<double>>& accounts_timeseries);

bool is_empty(std::ifstream& pFile);

std::vector<double> load_1d(std::string filename);

std::vector<std::vector<double>> load_2d(std::string filename);

#endif