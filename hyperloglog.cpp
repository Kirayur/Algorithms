#include "hyperloglog.h"
#include <algorithm>

HyperLogLog::HyperLogLog(uint8_t B) : B(B), m(1u << B), M(m, 0) {}

void HyperLogLog::add(uint32_t hash) {
    uint32_t idx = hash >> (32 - B);
    uint32_t w = hash << B;
    M[idx] = std::max(M[idx], rho(w));
}

uint8_t HyperLogLog::rho(uint32_t w) const {
    if (w == 0) return 32 - B + 1;
    uint8_t r = 1;
    while ((w & 0x80000000) == 0) {
        r++;
        w <<= 1;
    }
    return r;
}

double HyperLogLog::alphaMM() const {
    switch(m) {
        case 16: return 0.673 * m * m;
        case 32: return 0.697 * m * m;
        case 64: return 0.709 * m * m;
        default: return (0.7213 / (1 + 1.079 / m)) * m * m;
    }
}

double HyperLogLog::estimate() const {
    double sum = 0.0;
    for (auto v : M) sum += std::pow(2.0, -v);
    double raw = alphaMM() / sum;

    if (raw <= 2.5 * m) {
        int V = std::count(M.begin(), M.end(), 0);
        if (V != 0)
            raw = m * std::log(static_cast<double>(m)/V);
    }
    return raw;
}
