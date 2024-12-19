#include "day_8.hpp"
#include "day.hpp"

namespace AOC {

void Day8::process_line(const std::string &line) {
    if (m_grid.width == 0) {
        m_grid.width = line.length() - 1;
    }

    for (size_t i = 0; i < line.length() - 1; i++) {
        const char c = line[i];

        if (c == '.') {
            continue;
        }

        const size_t index = char_to_index(c);
        Point *points = m_points[index];

        const int32_t x_1 = static_cast<int32_t>(i);
        const auto y_1 = static_cast<int32_t>(m_grid.height);

        points[m_points_length[index]++] = {x_1, y_1};

        for (size_t j = 0; j < m_points_length[index] - 1; j++) {
            const int32_t x_2 = m_points[index][j].x;
            const int32_t y_2 = m_points[index][j].y;

            const int32_t dx = x_1 - x_2;
            const int32_t dy = y_1 - y_2;

            // PART 1

            const int32_t anti_1_x = x_2 - dx;
            const int32_t anti_1_y = y_2 - dy;

            const int32_t anti_2_x = x_1 + dx;
            const int32_t anti_2_y = y_1 + dy;

            if (anti_1_x >= 0 && static_cast<size_t>(anti_1_x) < m_grid.width &&
                anti_1_y >= 0 && static_cast<size_t>(anti_1_y) < m_grid.width) {
                m_result.part1 += m_grid.grid[anti_1_x][anti_1_y].mark_part_1();
            }
            if (anti_2_x >= 0 && static_cast<size_t>(anti_2_x) < m_grid.width &&
                anti_2_y >= 0 && static_cast<size_t>(anti_2_y) < m_grid.width) {
                m_result.part1 += m_grid.grid[anti_2_x][anti_2_y].mark_part_1();
            }

            int32_t x = x_2;
            int32_t y = y_2;

            while (x >= 0 && y >= 0 && static_cast<size_t>(x) < m_grid.width &&
                   static_cast<size_t>(y) < m_grid.width) {
                m_result.part2 += m_grid.grid[x][y].mark_part_2();
                x -= dx;
                y -= dy;
            }

            x = x_1;
            y = y_1;

            while (x >= 0 && y >= 0 && static_cast<size_t>(x) < m_grid.width &&
                   static_cast<size_t>(y) < m_grid.width) {
                m_result.part2 += m_grid.grid[x][y].mark_part_2();
                x += dx;
                y += dy;
            }
        }
    }

    m_grid.height++;
}
}  // namespace AOC
