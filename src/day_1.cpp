#include "day_1.hpp"
#include <cstdlib>

namespace AOC {
void Day1::process_line(const std::string &line) {
    uint32_t n = 0;
    size_t i = 0;
    for (; line[i] >= '0' && line[i] <= '9'; i++) {
        n = n * 10 + (line[i] - '0');
    }
    list_0.push_back(n);

    while (line[i] < '0' || line[i] > '9') {
        i++;
    }

    n = 0;
    for (; line[i] >= '0' && line[i] <= '9'; i++) {
        n = n * 10 + (line[i] - '0');
    }
    list_1.push_back(n);
}

void Day1::post_processing() {
    JC_PROFILE_FUNCTION();

    {
        JC_PROFILE_SCOPE("Sorting");
        std::ranges::sort(list_0);
        std::ranges::sort(list_1);
    }

    {
        JC_PROFILE_SCOPE("Part 1");
        for (size_t i = 0; i < list_0.size(); ++i) {
            const auto n_0 = static_cast<int32_t>(list_0[i]);
            const auto n_1 = static_cast<int32_t>(list_1[i]);

            m_result.part1 += std::abs(n_0 - n_1);
        }
    }

    {
        JC_PROFILE_SCOPE("Part 2");
        auto it = list_1.begin();

        uint32_t last_n = 0;
        uint32_t last_count = 0;

        for (const auto n : list_0) {

            if (n == last_n) {
                m_result.part2 += n * last_count;
                continue;
            }

            int count = 0;

            while (*it < n) {
                ++it;
            }

            while (*it == n) {
                ++count;
                ++it;
            }

            last_n = n;
            last_count = count;

            m_result.part2 += n * count;
        }
    }
}
}  // namespace AOC
