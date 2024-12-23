
#include "day_22.hpp"
namespace AOC {

static uint64_t mix(uint64_t a, uint64_t b) {
    return a ^ b;
}

static uint64_t prune(uint64_t a) {
    return a % 16777216;
}

static uint64_t next(uint64_t n) {
    n = prune(mix(n, n * 64));
    n = prune(mix(n, n / 32));
    n = prune(mix(n, n * 2048));
    return n;
}

void Day22::check_sequence(uint64_t *numbers, int32_t a, int32_t b, int32_t c,
                           int32_t d) {
    for (size_t i = 4; i < 2001; i++) {
        int32_t i_0 = static_cast<int32_t>(numbers[i]) % 10;
        int32_t i_1 = static_cast<int32_t>(numbers[i - 1]) % 10;

        if (i_0 - i_1 != d) {
            continue;
        }

        int32_t i_2 = static_cast<int32_t>(numbers[i - 2]) % 10;
        if (i_1 - i_2 != c) {
            continue;
        }

        int32_t i_3 = static_cast<int32_t>(numbers[i - 3]) % 10;
        if (i_2 - i_3 != b) {
            continue;
        }

        int32_t i_4 = static_cast<int32_t>(numbers[i - 4]) % 10;
        if (i_3 - i_4 != a) {
            continue;
        }

        sum_add(a, b, c, d, i_0);
        // trace("Sequence found: " << a << " " << b << " " << c << " " << d << " "
        //                          << i_0);
        return;
    }
}

void Day22::crime_against_programing(uint64_t *numbers) {
#pragma omp parallel for collapse(4) schedule(dynamic)
    for (int32_t a = -9; a <= 9; a++) {
        for (int32_t b = -9; b <= 9; b++) {
            for (int32_t c = -9; c <= 9; c++) {
                for (int32_t d = -9; d <= 9; d++) {
                    if (a + b + c + d < -9 || a + b + c + d > 9) {
                        continue;
                    }
                    check_sequence(numbers, a, b, c, d);
                }
            }
        }
    }
}

void Day22::process_line(const std::string &line) {
    trace_prefix << line;
    uint64_t n = 0;
    size_t i = 0;
    // Parse Number
    while (line[i] >= '0' && line[i] <= '9') {
        n = n * 10 + line[i] - '0';
        i++;
    }

    // Part 1
    uint64_t numbers[2001];
    numbers[0] = n;
    for (size_t i = 0; i < 2000; i++) {
        n = next(n);
        numbers[i + 1] = n;
    }
    m_result.part1 += n;

    // Part2
    crime_against_programing(numbers);
}

void Day22::post_processing() {
    uint64_t max = 0;
    for (int32_t a = -9; a <= 9; a++) {
        for (int32_t b = -9; b <= 9; b++) {
            for (int32_t c = -9; c <= 9; c++) {
                for (int32_t d = -9; d <= 9; d++) {
                    if (a + b + c + d < -9 || a + b + c + d > 9) {
                        continue;
                    }
                    uint32_t s = sum_get(a, b, c, d);
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
