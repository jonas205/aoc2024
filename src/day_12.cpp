
#include "day_12.hpp"

namespace AOC {

void Day12::process_line(const std::string &line) {
    if (m_grid.width == 0) {
        m_grid.width = line.length() - 1;
    }

    for (size_t i = 0; i < line.length() - 1; i++) {
        m_grid.cell[m_grid.height][i].type = line[i];
    }

    m_grid.height++;
}

Day12::Region Day12::find_region(const int32_t x, const int32_t y) {
    static constexpr int32_t modifiers[4][6] = {{-1, 0, 0, -1, -1, -1},
                                                {1, 0, 0, 1, 1, 1},
                                                {0, -1, 1, 0, 1, -1},
                                                {0, 1, -1, 0, -1, 1}};

    auto &[type, checked] = m_grid.cell[y][x];
    checked = true;
    Region erg{};
    erg.area = 1;
    erg.perimeter = 0;
    erg.sides = 0;

    for (const auto mod : modifiers) {
        Cell &other = cell_at(x + mod[0], y + mod[1]);
        if (other.type != type) {
            erg.perimeter += 1;

            Cell &nb = cell_at(x + mod[2], y + mod[3]);
            Cell &other_nb = cell_at(x + mod[4], y + mod[5]);

            if (!(nb.type == type && other_nb.type != type)) {
                erg.sides += 1;
            }

            continue;
        } else if (other.checked) {
            continue;
        }

        Region reg = find_region(x + mod[0], y + mod[1]);
        erg.area += reg.area;
        erg.perimeter += reg.perimeter;
        erg.sides += reg.sides;
    }

    return erg;
}

void Day12::post_processing() {
    for (size_t i = 0; i < m_grid.height; i++) {
        for (size_t j = 0; j < m_grid.width; j++) {
            if (m_grid.cell[i][j].checked) {
                continue;
            }
            Region reg = find_region(static_cast<int32_t>(j), static_cast<int32_t>(i));
            // JC_TRACE << m_grid.cell[i][j].type << ": Area: " << reg.area << " Perimeter: " << reg.perimeter
            //          << " Sides: " << reg.sides << '\n';
            m_result.part1 += reg.perimeter * reg.area;
            m_result.part2 += reg.sides * reg.area;
        }
    }
}

}  // namespace AOC
