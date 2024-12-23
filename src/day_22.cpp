
#include "day_22.hpp"
namespace AOC {

static uint64_t mix(uint64_t a, uint64_t b) {
    return a ^ b;
}

static uint64_t prune(uint64_t a) {
    return a & 0xFFFFFF;
}

static uint64_t next(uint64_t n) {
    n = prune(mix(n, n << 6));
    n = mix(n, n >> 5);
    n = prune(mix(n, n << 11));
    return n;
}

void Day22::process_line(const std::string &line) {
    // trace_prefix << line;
    uint64_t start = 0;
    size_t i = 0;
    // Parse Number
    while (line[i] >= '0' && line[i] <= '9') {
        start = start * 10 + line[i] - '0';
        i++;
    }

    // Part 1
    int32_t deltas[4] = {0};
    int32_t last = static_cast<int32_t>(start % 10);
    uint64_t n = start;
    for (size_t i = 0; i < 2000; i++) {
        n = next(n);
        deltas[3] = deltas[2];
        deltas[2] = deltas[1];
        deltas[1] = deltas[0];
        int32_t last_digit = static_cast<int32_t>(n % 10);
        deltas[0] = last_digit - last;
        last = last_digit;
        if (i < 4) {
            continue;
        }
        if (last_seen(deltas[0], deltas[1], deltas[2], deltas[3]) == start) {
            continue;
        }
        last_seen(deltas[0], deltas[1], deltas[2], deltas[3]) = start;
        sum(deltas[0], deltas[1], deltas[2], deltas[3]) += last_digit;
    }
    m_result.part1 += n;
}

void Day22::post_processing() {
    int32_t max = 0;
    for (int32_t a = -9; a <= 9; a++) {
        for (int32_t b = -9; b <= 9; b++) {
            for (int32_t c = -9; c <= 9; c++) {
                for (int32_t d = -9; d <= 9; d++) {
                    if (a + b + c + d < -9 || a + b + c + d > 9) {
                        continue;
                    }
                    int32_t s = sum(a, b, c, d);
                    if (s > max) {
                        max = s;
                    }
                }
            }
        }
    }
    m_result.part2 = max;
}

}  // namespace AOC
