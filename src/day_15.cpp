
#include "day_15.hpp"
namespace AOC {

void Day15::print_1() const {
    for (int32_t y = 0; y < static_cast<int32_t>(m_board.height); y++) {
        for (int32_t x = 0; x < static_cast<int32_t>(m_board.width); x++) {
            if (x == m_board.x && y == m_board.y) {
                std::cout << '@';
            } else {
                switch (m_board.cells[y][x]) {
                    case Cell::EMPTY:
                        std::cout << '.';
                        break;
                    case Cell::BOX:
                        std::cout << 'O';
                        break;
                    case Cell::WALL:
                        std::cout << '#';
                        break;
                    default:
                        break;
                }
            }
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

void Day15::print_2() const {
    std::cout << '+';
    for (int32_t x = 0; x < static_cast<int32_t>(m_board_2.width); x++) {
        std::cout << x % 10;
    }
    std::cout << '\n';

    for (int32_t y = 0; y < static_cast<int32_t>(m_board_2.height); y++) {
        std::cout << y % 10;
        for (int32_t x = 0; x < static_cast<int32_t>(m_board_2.width); x++) {
            if (x == m_board_2.x && y == m_board_2.y) {
                std::cout << '@';
            } else {
                switch (m_board_2.cells[y][x]) {
                    case Cell::EMPTY:
                        std::cout << '.';
                        break;
                    case Cell::BOX_LEFT:
                        std::cout << '[';
                        break;
                    case Cell::BOX_RIGHT:
                        std::cout << ']';
                        break;
                    case Cell::WALL:
                        std::cout << '#';
                        break;
                    default:
                        break;
                }
            }
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}
void Day15::move_2(Move move) {
    static constexpr int32_t deltas[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    const int32_t dx = deltas[static_cast<int>(move)][0];
    const int32_t dy = deltas[static_cast<int>(move)][1];

    int32_t &x = m_board_2.x;
    int32_t &y = m_board_2.y;

    if (m_board_2.cells[y + dy][x + dx] == Cell::EMPTY) {
        x += dx;
        y += dy;
        return;
    }
    if (m_board_2.cells[y + dy][x + dx] == Cell::WALL) {
        return;
    }

    if (can_move_box(move, x + dx, y + dy)) {
        move_box(move, x + dx, y + dy);

        x += dx;
        y += dy;
    }
}

bool Day15::can_move_box(const Move move, int32_t x, const int32_t y) {
    if (m_board_2.cells[y][x] == Cell::BOX_RIGHT) {
        x -= 1;
    }

    switch (move) {
        case Move::LEFT: {
            if (m_board_2.cells[y][x - 1] == Cell::EMPTY) {
                return true;
            }
            if (m_board_2.cells[y][x - 1] == Cell::BOX_RIGHT) {
                return can_move_box(move, x - 1, y);
            }
            return false;
        }
        case Move::RIGHT: {
            if (m_board_2.cells[y][x + 2] == Cell::EMPTY) {
                return true;
            }
            if (m_board_2.cells[y][x + 2] == Cell::BOX_LEFT) {
                return can_move_box(move, x + 2, y);
            }
            return false;
        }
        case Move::UP: {
            if (m_board_2.cells[y - 1][x] == Cell::EMPTY &&
                m_board_2.cells[y - 1][x + 1] == Cell::EMPTY) {
                return true;
            }
            if (m_board_2.cells[y - 1][x] == Cell::WALL ||
                       m_board_2.cells[y - 1][x + 1] == Cell::WALL) {
                return false;
            }
            if ((m_board_2.cells[y - 1][x] == Cell::BOX_LEFT ||
                 m_board_2.cells[y - 1][x] == Cell::BOX_RIGHT) &&
                !can_move_box(move, x, y - 1)) {
                return false;
            }
            if ((m_board_2.cells[y - 1][x + 1] == Cell::BOX_LEFT ||
                 m_board_2.cells[y - 1][x + 1] == Cell::BOX_RIGHT) &&
                !can_move_box(move, x + 1, y - 1)) {
                return false;
            }
            return true;
        }
        case Move::DOWN: {
            if (m_board_2.cells[y + 1][x] == Cell::EMPTY &&
                m_board_2.cells[y + 1][x + 1] == Cell::EMPTY) {
                return true;
            }
            if (m_board_2.cells[y + 1][x] == Cell::WALL ||
                       m_board_2.cells[y + 1][x + 1] == Cell::WALL) {
                return false;
            }
            if ((m_board_2.cells[y + 1][x] == Cell::BOX_LEFT ||
                 m_board_2.cells[y + 1][x] == Cell::BOX_RIGHT) &&
                !can_move_box(move, x, y + 1)) {
                return false;
            }
            if ((m_board_2.cells[y + 1][x + 1] == Cell::BOX_LEFT ||
                 m_board_2.cells[y + 1][x + 1] == Cell::BOX_RIGHT) &&
                !can_move_box(move, x + 1, y + 1)) {
                return false;
            }
            return true;
        }
    }

    return false;
}

void Day15::move_box(const Move move, int32_t x, const int32_t y) {
    if (m_board_2.cells[y][x] == Cell::BOX_RIGHT) {
        x -= 1;
    }

    switch (move) {
        case Move::LEFT: {
            if (m_board_2.cells[y][x - 1] == Cell::BOX_RIGHT) {
                move_box(move, x - 1, y);
            }
            m_board_2.cells[y][x - 1] = Cell::BOX_LEFT;
            m_board_2.cells[y][x] = Cell::BOX_RIGHT;
            m_board_2.cells[y][x + 1] = Cell::EMPTY;
            return;
        }
        case Move::RIGHT: {
            if (m_board_2.cells[y][x + 2] == Cell::BOX_LEFT) {
                move_box(move, x + 2, y);
            }
            m_board_2.cells[y][x + 2] = Cell::BOX_RIGHT;
            m_board_2.cells[y][x + 1] = Cell::BOX_LEFT;
            m_board_2.cells[y][x] = Cell::EMPTY;

            return;
        }
        case Move::UP: {
            if (m_board_2.cells[y - 1][x] == Cell::BOX_RIGHT ||
                m_board_2.cells[y - 1][x] == Cell::BOX_LEFT) {
                move_box(move, x, y - 1);
            }
            if (m_board_2.cells[y - 1][x + 1] == Cell::BOX_RIGHT ||
                m_board_2.cells[y - 1][x + 1] == Cell::BOX_LEFT) {
                move_box(move, x + 1, y - 1);
            }
            m_board_2.cells[y - 1][x] = Cell::BOX_LEFT;
            m_board_2.cells[y - 1][x + 1] = Cell::BOX_RIGHT;
            m_board_2.cells[y][x] = Cell::EMPTY;
            m_board_2.cells[y][x + 1] = Cell::EMPTY;
            return;
        }
        case Move::DOWN: {
            if (m_board_2.cells[y + 1][x] == Cell::BOX_RIGHT ||
                m_board_2.cells[y + 1][x] == Cell::BOX_LEFT) {
                move_box(move, x, y + 1);
            }
            if (m_board_2.cells[y + 1][x + 1] == Cell::BOX_RIGHT ||
                m_board_2.cells[y + 1][x + 1] == Cell::BOX_LEFT) {
                move_box(move, x + 1, y + 1);
            }
            m_board_2.cells[y + 1][x] = Cell::BOX_LEFT;
            m_board_2.cells[y + 1][x + 1] = Cell::BOX_RIGHT;
            m_board_2.cells[y][x] = Cell::EMPTY;
            m_board_2.cells[y][x + 1] = Cell::EMPTY;
            return;
        }
    }
}

void Day15::move_1(Move move) {
    static constexpr int32_t deltas[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    const int32_t dx = deltas[static_cast<int>(move)][0];
    const int32_t dy = deltas[static_cast<int>(move)][1];

    int32_t &x = m_board.x;
    int32_t &y = m_board.y;

    if (m_board.cells[y + dy][x + dx] == Cell::EMPTY) {
        x += dx;
        y += dy;
        return;
    }
    if (m_board.cells[y + dy][x + dx] == Cell::WALL) {
        return;
    }

    int32_t check_x = m_board.x + 2 * dx;
    int32_t check_y = m_board.y + 2 * dy;

    while (true) {
        if (m_board.cells[check_y][check_x] == Cell::WALL) {
            return;
        }
        if (m_board.cells[check_y][check_x] == Cell::BOX) {
            check_x += dx;
            check_y += dy;
        } else {
            m_board.cells[check_y][check_x] = Cell::BOX;
            m_board.cells[y + dy][x + dx] = Cell::EMPTY;
            x += dx;
            y += dy;
            return;
        }
    }
}

void Day15::process_line(const std::string &line) {
    // JC_TRACE << m_board_parsing << ' ' << line;

    if (m_board_parsing && line.length() == 1) {
        m_board_parsing = false;
        // print_2();
    } else if (m_board_parsing) {
        if (m_board.width == 0) {
            m_board.width = line.length() - 1;
            m_board_2.width = 2 * m_board.width;
        }

        for (size_t i = 0; i < line.length(); i++) {
            if (line[i] == '.') {
                m_board.cells[m_board.height][i] = Cell::EMPTY;
                m_board_2.cells[m_board.height][2 * i] = Cell::EMPTY;
                m_board_2.cells[m_board.height][2 * i + 1] = Cell::EMPTY;
            } else if (line[i] == 'O') {
                m_board.cells[m_board.height][i] = Cell::BOX;
                m_board_2.cells[m_board.height][2 * i] = Cell::BOX_LEFT;
                m_board_2.cells[m_board.height][2 * i + 1] = Cell::BOX_RIGHT;
            } else if (line[i] == '#') {
                m_board.cells[m_board.height][i] = Cell::WALL;
                m_board_2.cells[m_board.height][2 * i] = Cell::WALL;
                m_board_2.cells[m_board.height][2 * i + 1] = Cell::WALL;
            } else if (line[i] == '@') {
                m_board.cells[m_board.height][i] = Cell::EMPTY;
                m_board_2.cells[m_board.height][2 * i] = Cell::EMPTY;
                m_board_2.cells[m_board.height][2 * i + 1] = Cell::EMPTY;
                m_board.x = static_cast<int32_t>(i);
                m_board.y = static_cast<int32_t>(m_board.height);
                m_board_2.x = 2 * m_board.x;
                m_board_2.y = m_board.y;
            }
        }

        m_board.height++;
        m_board_2.height++;
    } else {
        for (size_t i = 0; i < line.length(); i++) {
            switch (line[i]) {
                case '<':
                    move_1(Move::LEFT);
                    move_2(Move::LEFT);
                    break;
                case '>':
                    move_1(Move::RIGHT);
                    move_2(Move::RIGHT);
                    break;
                case '^':
                    move_1(Move::UP);
                    move_2(Move::UP);
                    break;
                case 'v':
                    move_1(Move::DOWN);
                    move_2(Move::DOWN);
                    break;
                default:;
            }
            // JC_TRACE << "Move: " << line[i] << '\n';
            // print_2();
        }
    }
}
void Day15::post_processing() {
    for (size_t i = 0; i < m_board.height; i++) {
        for (size_t j = 0; j < m_board.width; j++) {
            if (m_board.cells[i][j] == Cell::BOX) {
                m_result.part1 += 100 * i + j;
            }
        }
        for (size_t j = 0; j < m_board_2.width; j++) {
            if (m_board_2.cells[i][j] == Cell::BOX_LEFT) {
                m_result.part2 += 100 * i + j;
            }
        }
    }
}

}  // namespace AOC
