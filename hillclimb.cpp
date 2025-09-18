#include <vector>
#include <random>
#include <functional>
using std::vector;
using std::function;

template <typename T>
T StochasticHillClimb(vector<T> domain, float epsilon, function<T(T, vector<T>, float)> distance,
                      function<float(T)> fitness, const function<bool()>& stopCondition)
{
    std::random_device rnd;
    std::mt19937 gen{rnd()};
    std::uniform_int_distribution<> range = new std::uniform_int_distribution(0, domain.size() - 1);
    T element = domain[range(gen)];
    while (!stopCondition())
    {
        auto neighbour = distance(element, domain, epsilon);
        if (fitness(neighbour) < fitness(element))
        {
            element = neighbour;
        }
    }

    return element;
}

template <typename T>
T SteepestAscentHillClimb(vector<T> domain, float epsilon, function<vector<T>(T, vector<T>, float)> distance,
                          function<float(T)> fitness, const function<bool()>& stopCondition)
{
    std::random_device rnd;
    std::mt19937 gen(rnd());
    std::uniform_int_distribution<> range(0, domain.size() - 1);

    T element = domain[range(gen)];
    auto stuck = false;
    while (!stuck && !stopCondition())
    {
        int opt = 0;
        auto neighbours = distance(element, domain, epsilon);
        auto optFitness = fitness(neighbours[opt]);
        for (size_t i = 0; i < neighbours.size(); i++)
        {
            auto currFitness = fitness(neighbours[i]);
            if (currFitness < optFitness)
            {
                opt = i;
                optFitness = currFitness;
            }
        }

        if (optFitness < fitness(element))
        {
            element = neighbours[opt];
        }
        else
        {
            stuck = true;
        }
    }

    return element;
}
