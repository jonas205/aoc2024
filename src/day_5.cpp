#include "day_5.hpp"

namespace AOC {

void Day5::map_line(const std::string &line) {
    uint32_t n_0 = 10 * (line[0] - '0');
    n_0 += line[1] - '0';

    uint32_t n_1 = 10 * (line[3] - '0');
    n_1 += line[4] - '0';

    const uint32_t len = ++m_order.order[n_0][0];
    m_order.order[n_0][len] = n_1;
}

void Day5::check_line() {
    for (size_t i = 0; i < m_line_length; i++) {
        const uint32_t number = m_line[i];

        if (m_order.order[number][0] == 0) {
            continue;
        }

        for (uint32_t k = 1; k <= m_order.order[number][0]; k++) {
            uint32_t v = m_order.order[number][k];
            for (size_t j = 0; j < i; j++) {
                if (m_line[j] != v) {
                    continue;
                }

                std::sort(std::begin(m_line),
                          std::begin(m_line) + m_line_length, m_sorter);

                const size_t middle = m_line_length / 2;
                m_result.part2 += m_line[middle];
                return;
            }
        }
    }

    const size_t middle = m_line_length / 2;
    m_result.part1 += m_line[middle];
}

void Day5::process_line(const std::string &line) {
    if (line[0] == '\n') {
        m_mapping_mode = false;
    } else if (m_mapping_mode) {
        map_line(line);
    } else {
        m_line_length = 0;
        for (size_t i = 0; i + 2 <= line.size(); i += 3) {
            m_line[m_line_length] = 10 * (line[i] - '0');
            m_line[m_line_length] += line[i + 1] - '0';
            m_line_length++;
        }

        check_line();
    }
}
}  // namespace AOC
