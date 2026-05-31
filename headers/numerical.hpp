#ifndef NUMERICAL_H
#define NUMERICAL_H

#include <iostream>
#include <numeric>
#include <random>
#include <vector>
#include <algorithm>

double nominalFromEffective(double effective, int convertible);

double standardDeviation(const std::vector<double>& values);

double mean(const std::vector<double> &v);

double median(std::vector<double> v);

double toPennies(double x);

#endif