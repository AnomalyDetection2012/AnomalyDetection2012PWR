#include <cmath>
#include <vector>

double euclideanSpace(const std::vector<double> &v1, const std::vector<double> &v2)
{
    double result = 0.0;
    for (int i = 0; i < v1.size(); i++) result += pow((v1[i] - v2[i]), 2.0);
    return sqrt(result);
}
