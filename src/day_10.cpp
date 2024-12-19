
#include "day_10.hpp"
namespace AOC {

void Day10::process_line(const std::string &line) {
    if (m_grid.width == 0) {
        m_grid.width = line.length() - 1;
    }

    for (size_t i = 0; i < line.length() - 1; i++) {
        m_grid.cells[m_grid.height][i].value = static_cast<int8_t>(line[i] - '0');
    }

    m_grid.height++;
}

void Day10::next(const size_t x, const size_t y, bool fresh) {
    auto [value, last_seen] = m_grid.cells[y][x];

    if (last_seen == m_grid.current_0) {
        fresh = false;
    }

    m_grid.cells[y][x].last_seen = m_grid.current_0;
    if (value == 9) {
        if (fresh) {
            m_result.part1++;
        }
        m_result.part2++;
        return;
    }

    if (get(x + 1, y) == value + 1) {
        next(x + 1, y, fresh);
    }
    if (get(x - 1, y) == value + 1) {
        next(x - 1, y, fresh);
    }
    if (get(x, y + 1) == value + 1) {
        next(x, y + 1, fresh);
    }
    if (get(x, y - 1) == value + 1) {
        next(x, y - 1, fresh);
    }
}

void Day10::post_processing() {
    for (size_t y = 0; y < m_grid.height; y++) {
        for (size_t x = 0; x < m_grid.width; x++) {
            if (m_grid.cells[y][x].value == 0) {
                m_grid.current_0++;
                next(x, y);
            }
        }
    }
}

}  // namespace AOC
