#include "numerical.hpp"

double nominalFromEffective(double effective, int convertible) {
    return (pow((1 + effective), 1.0/static_cast<double>(convertible)) - 1);
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


double mean(const std::vector<double> &v) {
    return std::accumulate(v.begin(), v.end(), 0.0) / static_cast<double>(v.size());
}


double median(std::vector<double> v) {
    size_t n = v.size() / 2;
    nth_element(v.begin(), v.begin()+n, v.end());
    return v[n];
}


double toPennies(double x) {
    return std::round(x * 100.00) / 100;
}