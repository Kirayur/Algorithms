#pragma once
#include <vector>
#include <cstdint>
#include <cmath>

class HyperLogLog {
public:
    explicit HyperLogLog(uint8_t B);

    void add(uint32_t hash);
    double estimate() const;

private:
    uint8_t B;
    uint32_t m;
    std::vector<uint8_t> M;

    uint8_t rho(uint32_t w) const;
    double alphaMM() const;
};
