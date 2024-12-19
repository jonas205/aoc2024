#include "day_11.hpp"
namespace AOC {

static uint64_t cache[128][512][2] = {{{0}}}; // While it does not fit in the L1 cache, it is the fastest size I found
static constexpr uint64_t cache_mask = 0b0111111111;

static uint64_t apply(const uint64_t number, const size_t depth) {
    if (depth == 0) {
        return 1;
    }

    if (number != 0 && cache[depth][number & cache_mask][0] == number) {
        return cache[depth][number & cache_mask][1];
    }

    if (number == 0) {
        const uint64_t erg = apply(1, depth - 1);
        cache[depth][number & cache_mask][0] = number;
        cache[depth][number & cache_mask][1] = erg;
        return erg;
    }

    size_t count = 0;
    uint64_t n = number;
    while (n) {
        n /= 10;
        count++;
    }

    if (count % 2 == 0) {
        size_t divider = 1;
        for (size_t i = 0; i < count / 2; i++) {
            divider *= 10;
        }
        uint64_t erg = apply(number / divider, depth - 1);
        erg += apply(number % divider, depth - 1);
        cache[depth][number & cache_mask][0] = number;
        cache[depth][number & cache_mask][1] = erg;
        return erg;
    }

    const uint64_t erg = apply(number * 2024, depth - 1);
    cache[depth][number & cache_mask][0] = number;
    cache[depth][number & cache_mask][1] = erg;
    return erg;
}

void Day11::calculate(const std::string &content) {
    for (size_t i = 0; i < content.length() - 1; i++) {
        uint64_t number = 0;

        while (content[i] != ' ' && content[i] != '\n') {
            number = 10 * number + content[i] - '0';
            i++;
        }

        m_result.part1 += apply(number, 25);
        m_result.part2 += apply(number, 75);
    }
}
}  // namespace AOC
