#include "day_4.hpp"

namespace AOC {
void Day4::process_line(const std::string &line) {
    if (m_grid.width == 0) {
        m_grid.width = line.length();
    }
    for (size_t i = 0; i < m_grid.width; i++) {
        m_grid.grid[i][m_grid.height] = line[i];
    }
    m_grid.height++;
}

static void part1(Result &result, Day4::Grid &grid, const size_t i,
                  const size_t j) {
    constexpr char word[] = {'M', 'A', 'S'};
    constexpr int32_t mods[8][2] = {
        {-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1},
    };

    for (auto *mod : mods) {
        bool success = true;
        for (int32_t k = 0; k < static_cast<int32_t>(sizeof(word)); k++) {
            const int32_t x = (1 + k) * mod[0] + static_cast<int32_t>(i);
            const int32_t y = (1 + k) * mod[1] + static_cast<int32_t>(j);

            if (grid.get(x, y) != word[k]) {
                success = false;
                break;
            }
        }
        if (success) {
            result.part1++;
        }
    }
}

static void part2(Result &result, Day4::Grid &grid, const size_t i,
                  const size_t j) {
    if (((grid.get(i - 1, j - 1) == 'M' && grid.get(i + 1, j + 1) == 'S') ||
         (grid.get(i - 1, j - 1) == 'S' && grid.get(i + 1, j + 1) == 'M')) &&
        ((grid.get(i + 1, j - 1) == 'M' && grid.get(i - 1, j + 1) == 'S') ||
         (grid.get(i + 1, j - 1) == 'S' && grid.get(i - 1, j + 1) == 'M'))) {
        result.part2++;
    }
}

void Day4::post_processing() {
    for (size_t i = 0; i < m_grid.width; i++) {
        for (size_t j = 0; j < m_grid.height; j++) {
            // Part 1
            if (m_grid.get(i, j) == 'X') {
                part1(m_result, m_grid, i, j);
            }
            if (m_grid.get(i, j) == 'A') {
                part2(m_result, m_grid, i, j);
            }
        }
    }
}
}  // namespace AOC
