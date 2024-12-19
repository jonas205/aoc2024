#include "day_19.hpp"
namespace AOC {

void Day19::map_towels(const std::string &line) {
    size_t i = 0;
    while (i < line.size()) {
        const char c = line[i];
        if (c == '\n') {
            m_towel_size++;
            return;
        }
        if (c == ',') {
            i += 2;
            m_towel_size++;
            continue;
        }
        m_towel[m_towel_size][m_towel_sizes[m_towel_size]++] = c;
        i++;
    }
}

size_t Day19::is_possible(const std::string &line, const size_t pos,
                          size_t *line_cache) {
    if (pos >= line.size() || line[pos] == '\n') {
        return 1;
    }
    if (line_cache[pos] != SIZE_MAX) {
        return line_cache[pos];
    }

    size_t erg = 0;

    for (size_t i = 0; i < m_towel_size; i++) {
        const char *towel = m_towel[i];

        bool possible = true;
        for (size_t j = 0; j < m_towel_sizes[i]; j++) {
            const char c = towel[j];
            if (c == '\n' || c != line[pos + j]) {
                possible = false;
                break;
            }
        }
        if (!possible) {
            continue;
        }

        erg += is_possible(line, pos + m_towel_sizes[i], line_cache);
    }

    line_cache[pos] = erg;

    return erg;
}

void Day19::process_line(const std::string &line) {
    if (m_line_count == 0) {
        map_towels(line);
        m_line_count++;
        return;
    }
    if (m_line_count == 1) {
        m_line_count++;

        // for (size_t i = 0; i < m_towel_size; i++) {
        //     for (size_t j = 0; j < m_towel_sizes[i]; j++) {
        //         std::cout << static_cast<char>(m_towel[i][j]) << " ";
        //     }
        //     std::cout << std::endl;
        // }

        return;
    }

    m_lines[m_lines_count++] = line;
    m_line_count++;
}

void Day19::post_processing() {
#pragma omp parallel for
    for (size_t i = 0; i < m_lines_count; i++) {
        const std::string &line = m_lines[i];

        size_t m_line_cache[64]{};
        for (unsigned long &c : m_line_cache) {
            c = SIZE_MAX;
        }
        const size_t erg = is_possible(line, 0, m_line_cache);

#pragma omp critical
        {
            if (erg > 0) {
                m_result.part1++;
            }
            m_result.part2 += erg;
        }
    }
}

}  // namespace AOC
