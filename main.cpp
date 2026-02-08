#include "random_generate.h"
#include "hash_func.h"
#include "hyperloglog.h"

#include <iostream>
#include <vector>
#include <unordered_set>
#include <cmath>
#include <numeric>

size_t exactDistinct(const std::vector<std::string>& data) {
    std::unordered_set<std::string> s(data.begin(), data.end());
    return s.size();
}

int main() {
    const size_t STREAM_SIZE = 100000;
    const int NUM_STREAMS = 30;
    const uint8_t B = 10;

    std::vector<int> steps;
    for (int p = 5; p <= 100; p += 5)
        steps.push_back(p);

    HashFuncGen hasher;

    std::vector<double> exact(steps.size(), 0.0);
    std::vector<double> mean(steps.size(), 0.0);
    std::vector<double> sq_sum(steps.size(), 0.0);

    for (int k = 0; k < NUM_STREAMS; ++k) {
        RandomStreamGen gen(STREAM_SIZE, 42 + k);
        const auto& stream = gen.getStream();

        for (size_t i = 0; i < steps.size(); ++i) {
            int percent = steps[i];
            size_t len = stream.size() * percent / 100;

            HyperLogLog hll(B);
            std::vector<std::string> prefix;
            prefix.reserve(len);

            for (size_t j = 0; j < len; ++j) {
                prefix.push_back(stream[j]);
                hll.add(hasher.hash(stream[j]));
            }

            double est = hll.estimate();

            if (k == 0) {
                exact[i] = exactDistinct(prefix);
            }
            
            mean[i] += est;
            sq_sum[i] += est * est;
        }
    }

    std::cout << "percent, exact, mean, std\n";

    for (size_t i = 0; i < steps.size(); ++i) {
        mean[i] /= NUM_STREAMS;
        double variance = sq_sum[i] / NUM_STREAMS - mean[i] * mean[i];
        double stddev = std::sqrt(variance);

        std::cout << steps[i] << ", "
                  << exact[i] << ", "
                  << mean[i] << ", "
                  << stddev << "\n";
    }

    return 0;
}
