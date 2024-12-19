#include "day_7.hpp"
#include "day.hpp"

namespace AOC {

bool Day7::part_1(const uint64_t target, const uint64_t rem, const size_t i) {
    if (i == 0) {
        return rem == m_line[0];
    }

    const uint64_t erg = rem / m_line[i];
    const uint64_t erg_rem = rem % m_line[i];

    if (erg_rem == 0 && part_1(target, erg, i - 1)) {
        return true;
    }
    if (rem - m_line[i] > 0 && part_1(target, rem - m_line[i], i - 1)) {
        return true;
    }

    return false;
}

bool Day7::part_2(const uint64_t target, const uint64_t rem, const size_t i) {
    if (i == 0) {
        return rem == m_line[0];
    }

    uint64_t erg = rem / m_line[i];
    uint64_t erg_rem = rem % m_line[i];

    if (erg_rem == 0 && part_2(target, erg, i - 1)) {
        return true;
    }
    if (rem - m_line[i] > 0 && part_2(target, rem - m_line[i], i - 1)) {
        return true;
    }

    if (m_line[i] > 99) {
        erg = rem / 1000;
        erg_rem = rem % 1000;
    } else if (m_line[i] > 9) {
        erg = rem / 100;
        erg_rem = rem % 100;
    } else {
        erg = rem / 10;
        erg_rem = rem % 10;
    }

    if (erg_rem == m_line[i] && part_2(target, erg, i - 1)) {
        return true;
    }

    return false;
}

void Day7::process_line(const std::string &line) {
    size_t i = 0;

    uint64_t sum = 0;

    while (line[i] >= '0' && line[i] <= '9') {
        sum = 10 * sum + line[i] - '0';
        i++;
    }
    i += 2;

    m_line_length = 0;
    while (true) {
        uint32_t num = 0;
        while (line[i] >= '0' && line[i] <= '9') {
            num = 10 * num + line[i] - '0';
            i++;
        }
        m_line[m_line_length++] = num;
        if (line[i++] == '\n') {
            break;
        }
    }

    if (part_1(sum, sum, m_line_length - 1)) {
        m_result.part1 += sum;
        m_result.part2 += sum;
    } else if (part_2(sum, sum, m_line_length - 1)) {
        m_result.part2 += sum;
    }
}

}  // namespace AOC
