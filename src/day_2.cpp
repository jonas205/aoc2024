#include "day_2.hpp"
#include <sstream>

namespace AOC {

static size_t problem_at(const std::vector<int32_t> &numbers,
                         const size_t skip_i = -1) {
    int32_t number = skip_i == 0 ? numbers[1] : numbers[0];
    const int32_t second = skip_i < 2 ? numbers[2] : numbers[1];
    const bool increasing = second > number;

    for (size_t i = skip_i == 0 ? 2 : 1; i < numbers.size(); i++) {
        if (i == skip_i) {
            continue;
        }
        if (increasing && numbers[i] <= number) {
            return i;
        }
        if (!increasing && numbers[i] >= number) {
            return i;
        }

        if (std::abs(numbers[i] - number) > 3) {
            return i;
        }

        number = numbers[i];
    }
    return 0;
}

void Day2::process_line(const std::string &line) {
    std::vector<int32_t> numbers;

    for (size_t i = 0; i < line.size(); i++) {
        if (line[i] >= '0' && line[i] <= '9') {
            int32_t number = line[i] - '0';
            i++;
            while (line[i] >= '0' && line[i] <= '9') {
                number = number * 10 + (line[i] - '0');
                i++;
            }
            numbers.push_back(number);
        }
    }

    const size_t problem = problem_at(numbers);
    if ( problem == 0) {
        m_result.part1++;
        m_result.part2++;
    } else {
        for (size_t i = (static_cast<int>(problem) - 2 >= 0) ? problem - 2 : 0; i < (problem + 1 < numbers.size() ? problem + 1 : numbers.size()); i++) {
            if (problem_at(numbers, i) == 0) {
                m_result.part2++;
                break;
            }
        }
    }
}

}  // namespace AOC
