
#include "day_18.hpp"
#include "day.hpp"
namespace AOC {

void Day18::process_line(const std::string &line) {
    size_t x = 0, i = 0;
    while (line[i] >= '0' && line[i] <= '9') {
        x = x * 10 + line[i] - '0';
        i++;
    }
    i++;
    size_t y = 0;
    while (line[i] >= '0' && line[i] <= '9') {
        y = y * 10 + line[i] - '0';
        i++;
    }

    if (m_fallen < S_LIMIT) {
        m_board.cells[y][x].type = CellType::WALL;
        m_fallen++;
        return;
    } else if (m_fallen == S_LIMIT) {
        m_board.copy().solve(m_result.part1);
        m_fallen++;
        return;
    }

    m_points[m_points_size++] = {x, y};
}

void Day18::Board::print() {
    for (size_t y = 0; y < S_SIZE; ++y) {
        for (size_t x = 0; x < S_SIZE; ++x) {
            switch (cells[y][x].type) {
                case CellType::WALL:
                    std::cout << '#';
                    break;
                case CellType::NEW:
                    if (cells[y][x].cost < UINT32_MAX) {
                        std::cout << cells[y][x].cost % 10;
                    } else {
                        std::cout << '.';
                    }
                    break;
            }
        }
        std::cout << '\n';
    }
}

Day18::Board Day18::Board::copy() const {
    Board other;

    for (size_t y = 0; y < S_SIZE; ++y) {
        for (size_t x = 0; x < S_SIZE; ++x) {
            other.cells[y][x] = cells[y][x];
        }
    }

    return other;
}
bool Day18::Board::solve(uint64_t &result) {
    static constexpr size_t POINTS = 256;
    Point points[POINTS];
    size_t first = 0, last = 0, count = 0;

    auto points_push = [&](size_t x, size_t y) {
        JC_ASSERT(count < POINTS, "Points buffer overflow");
        points[last] = {x, y};
        last = (last + 1) % POINTS;
        count++;
    };

    auto points_pop = [&](size_t &x, size_t &y) {
        JC_ASSERT(count > 0, "Points buffer underflow");
        x = points[first].x;
        y = points[first].y;
        first = (first + 1) % POINTS;
        count--;
    };

    points_push(0, 0);
    cells[0][0].cost = 0;

    while (count != 0) {
        static constexpr int32_t deltas[4][2] = {
            {0, 1},
            {1, 0},
            {0, -1},
            {-1, 0},
        };

        size_t x, y;
        points_pop(x, y);
        size_t cost = cells[y][x].cost + 1;

        for (auto &d : deltas) {
            const int32_t n_x = x + d[0];
            const int32_t n_y = y + d[1];

            Cell &n = get(n_x, n_y);
            if (n.type == CellType::WALL) {
                continue;
            }

            if (n.cost > cost) {
                n.cost = cost;

                if (n_x == S_SIZE - 1 && n_y == S_SIZE - 1) {
                    result = cost;
                    return true;
                }

                points_push(n_x, n_y);
                // JC_TRACE << "New point: " << n_x << " " << n_y << '\n';
                // print();
            }
        }
    }

    return false;
}

void Day18::post_processing() {
    // trace("Points:  << m_points_size);
    size_t i = m_points_size / 2;
    size_t delta = m_points_size / 2;

    while (true) {
        Board board = m_board.copy();
        for (size_t j = 0; j < i; ++j) {
            const auto [x, y] = m_points[j];
            board.cells[y][x].type = CellType::WALL;
        }
        uint64_t foo;
        bool solvable = board.solve(foo);

        if (delta == 1 && solvable) {
            const auto [x, y] = m_points[i];
            m_result.part2 = 10000 * x + y;
            break;
        } else if (delta == 1) {
            const auto [x, y] = m_points[i - 1];
            m_result.part2 = 10000 * x + y;
            break;
        } else if (solvable) {
            delta /= 2;
            i += delta;
        } else {
            delta /= 2;
            i -= delta;
        }
    }
}
}  // namespace AOC
