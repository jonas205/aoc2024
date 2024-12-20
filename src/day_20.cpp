
#include "day_20.hpp"
#include <cstdlib>
#include "day.hpp"
namespace AOC {

void Day20::Board::print() {
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            switch (cells[y][x].state) {
                case CellState::Wall: {
                    std::cout << '#';
                    break;
                }
                case CellState::Empty: {
                    if (cells[y][x].value != UINT32_MAX) {
                        std::cout << cells[y][x].value % 10;
                    } else {
                        std::cout << '.';
                    }
                    break;
                }
            }
        }
        std::cout << std::endl;
    }
}

void Day20::Board::walk_board(Day20 &day, void (*callback)(Board &, Day20 &,
                                                           int32_t, int32_t)) {
    static constexpr int32_t deltas[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

    int32_t x = start_x, y = start_y;

    int32_t came_from_x = 0, came_from_y = 0;

    callback(*this, day, x, y);
    while (x != end_x || y != end_y) {
        for (const auto delta : deltas) {
            if (delta[0] == came_from_x && delta[1] == came_from_y) {
                continue;
            }
            if (cells[y + delta[1]][x + delta[0]].state == CellState::Empty) {
                callback(*this, day, x + delta[0], y + delta[1]);
                came_from_x = -delta[0];
                came_from_y = -delta[1];

                x += delta[0];
                y += delta[1];
                break;
            }
        }
    }

    (void)callback;
}

void day20_value_assigner(Day20::Board &board, Day20 &day, int32_t x,
                          int32_t y) {
    (void)day;

    if (x == board.start_x && y == board.start_y) {
        board.last_value = 0;
    } else {
        board.last_value++;
    }

    board.points[board.points_length++] = {x, y, board.last_value};

    board.cells[y][x].value = board.last_value;
}

void day20_cheater(Day20::Board &board, Day20 &day, int32_t x, int32_t y) {
    static constexpr int32_t deltas[][2] = {{0, -2}, {0, 2}, {-2, 0}, {2, 0}};
    uint32_t value = board.get(x, y);

    for (const auto delta : deltas) {
        uint32_t other = board.get(x + delta[0], y + delta[1]);
        if (other == UINT32_MAX || other <= value + 2) {
            continue;
        }
        uint32_t saved = other - value - 2;
        if (saved < 100) {
            continue;
        }
        day.m_result.part1 += 1;
    }
}

void Day20::process_line(const std::string &line) {
    if (m_board.width == 0) {
        m_board.width = line.size() - 1;
    }

    for (size_t i = 0; i < line.size() - 1; i++) {
        switch (line[i]) {
            case '#': {
                m_board.cells[m_board.height][i].state = CellState::Wall;
                break;
            }
            case '.': {
                m_board.cells[m_board.height][i].state = CellState::Empty;
                break;
            }
            case 'S': {
                m_board.cells[m_board.height][i].state = CellState::Empty;
                m_board.cells[m_board.height][i].value = 0;
                m_board.start_x = i;
                m_board.start_y = m_board.height;
                break;
            }
            case 'E': {
                m_board.cells[m_board.height][i].state = CellState::Empty;
                m_board.end_x = i;
                m_board.end_y = m_board.height;
                break;
            }
        }
    }
    m_board.height++;
}
void Day20::post_processing() {
    {
        JC_PROFILE_SCOPE("Value assigner");
        m_board.walk_board(*this, day20_value_assigner);
    }
    {
        JC_PROFILE_SCOPE("Cheater");
        m_board.walk_board(*this, day20_cheater);
    }
    {
        JC_PROFILE_SCOPE("Better Cheater");
        uint64_t sum = 0;

#pragma omp parallel for reduction(+ : sum)
        for (size_t i = 0; i < m_board.points_length; i++) {
            Point &p = m_board.points[i];
            for (size_t j = i + 21; j < m_board.points_length; j++) {
                Point &p2 = m_board.points[j];
                int32_t distance = std::abs(p.x - p2.x) + std::abs(p.y - p2.y);
                if (distance > 20) {
                    int32_t skip = distance - 21;
                    j += skip > 0 ? skip : 0;
                    continue;
                }
                uint32_t saved = p2.value - p.value - distance;
                if (saved < 100) {
                    continue;
                }
                sum += 1;
            }
        }
#pragma omp atomic
        m_result.part2 += sum;
    }
}
}  // namespace AOC
