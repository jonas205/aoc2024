
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

void day20_better_cheater(Day20::Board &board, Day20 &day, int32_t x,
                          int32_t y) {
    uint32_t value = board.get(x, y);

#define MIN(x, y) ((x < y) ? x : y)
#define MAX(x, y) ((x > y) ? x : y)
    for (int32_t y2 = MAX(y - 20, 0);
         y2 <= MIN(y + 20, static_cast<int32_t>(board.height)); y2++) {
        int32_t dy_abs = std::abs(y - y2);
        for (int32_t x2 = MAX(x - 20 + dy_abs, 0);
             x2 <= MIN(x + 20 - dy_abs, static_cast<int32_t>(board.width));
             x2++) {
            uint32_t other = board.get(x2, y2);
            int32_t distance = dy_abs + std::abs(x - x2);
            if (other == UINT32_MAX || other <= value + distance) {
                continue;
            }
            uint32_t saved = other - value - distance;
            if (saved < 100) {
                continue;
            }
            // JC_COUT << saved << ',';
            day.m_result.part2 += 1;
        }
    }
#undef MIN
#undef MAX
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
    m_board.walk_board(*this, day20_value_assigner);
    m_board.walk_board(*this, day20_cheater);
    m_board.walk_board(*this, day20_better_cheater);
}
}  // namespace AOC
