#include <cmath>
#include <random>
#include <vector>
#include <cassert>

namespace maxcut {

class MathHelper
{
public:
    static MathHelper& getInstance()
    {
        static MathHelper instance;
        return instance;
    }
    
    MathHelper() : _engine(std::random_device{}()), _exp(1), _unif() {}

    MathHelper(MathHelper const&)               = delete;
    void operator=(MathHelper const&)  = delete;

    void setPowerLawParam(double lambda) {
        _exp.param(std::exponential_distribution<>::param_type(lambda));
    }

    void setUniformRange(int a, int b) {
        _unif.param(std::uniform_int_distribution<>::param_type(a, b));
    }

    int getIntFromPowerLawDistribution(int max) {
        double rand = _exp(_engine);
        rand = rand > max ? max : rand;
        return ceil(rand);
    }

    std::vector<int> chooseKUnique(std::vector<int> population, std::vector<int> weights, int k) {
        std::vector<int> result;
        assert(population.size() == weights.size());
        int total_weight = 0;
        for(auto& v : weights) {
            total_weight += v;
        }
        std::vector<unsigned int> node_chosen(population.size(), false);
        for(int i = 0; i < k; ++i) {
            int r = _unif(_engine);
            int cumulative_weight = 0;
            for(int j = 0; j < population.size(); ++j) {
                cumulative_weight += weights[j];
                if(cumulative_weight >= r && !node_chosen[j]) {
                    result.push_back(population[j]);
                    node_chosen[j] = true;
                    break;
                }
            }
        }
        return result;
    }

private:
    std::mt19937 _engine;
    std::exponential_distribution<> _exp;
    std::uniform_int_distribution<> _unif;
};

}
