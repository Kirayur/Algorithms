#include "random_stream_gen.h"

RandomStreamGen::RandomStreamGen(size_t streamSize, uint32_t seed) : rng(seed) {
    stream.reserve(streamSize);
    for (size_t i = 0; i < streamSize; ++i) {
        stream.push_back(randomString());
    }
}

const std::vector<std::string>& RandomStreamGen::getStream() const {
    return stream;
}

std::vector<std::string> RandomStreamGen::getPrefix(double percent) const {
    size_t len = static_cast<size_t>(stream.size() * percent / 100.0);
    return std::vector<std::string>(stream.begin(), stream.begin() + len);
}

std::string RandomStreamGen::randomString() {
    static const std::string alphabet =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789-";
    std::uniform_int_distribution<int> len_dist(1, 30);
    std::uniform_int_distribution<int> char_dist(0, alphabet.size() - 1);

    int len = len_dist(rng);
    std::string s(len, ' ');
    for (int i = 0; i < len; ++i)
        s[i] = alphabet[char_dist(rng)];
    return s;
}
