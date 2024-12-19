#include "day_16.hpp"
#include "day.hpp"
namespace AOC {

void Day16::process_line(const std::string &line) {
    if (m_board.width == 0) {
        m_board.width = line.length() - 1;
    }

    for (size_t i = 0; i < line.length() - 1; i++) {
        switch (line[i]) {
            case '#': {
                m_board.cells[m_board.height][i].state = CellState::WALL;
                break;
            }
            case '.': {
                m_board.cells[m_board.height][i].state = CellState::FLOOR;
                break;
            }
            case 'S': {
                m_board.cells[m_board.height][i].state = CellState::FLOOR;
                m_board.start_x = i;
                m_board.start_y = m_board.height;
                break;
            }
            case 'E': {
                m_board.cells[m_board.height][i].state = CellState::FLOOR;
                m_board.end_x = i;
                m_board.end_y = m_board.height;
                break;
            }
            default: {
                die("Unknown character: " << line[i]);
                break;
            }
        }
    }

    m_board.height += 1;
}

enum class Direction : uint8_t {
    UP = 0,
    DOWN = 1,
    LEFT = 2,
    RIGHT = 3,
};

static constexpr int32_t DELTAS[4][8] = {
    {0, -1, -1, 0, 2, 1, 0, 3},
    {0, 1, 1, 0, 3, -1, 0, 2},
    {-1, 0, 0, 1, 1, 0, -1, 0},
    {1, 0, 0, -1, 0, 0, 1, 1},
};

static const int32_t *deltas(Direction dir) {
    return DELTAS[static_cast<uint8_t>(dir)];
}

struct Route {
    size_t length = 0;
    size_t turns = 0;
    int32_t x = 0, y = 0;
    Direction direction = Direction::UP;
};

void Day16::post_processing() {
    // m_board.print();

    Route route_a[128] = {{0}};
    Route route_b[128] = {{0}};

    Route *current = route_a;
    Route *next = route_b;

    size_t current_length = 0;
    size_t next_length = 0;

    static constexpr int32_t START_DIRECTIONS[4][3] = {
        {0, -1, 0},
        {0, 1, 1},
        {-1, 0, 2},
        {1, 0, 3},
    };

    m_board.visit(m_board.start_x, m_board.start_y, 0);

    for (const auto &dir : START_DIRECTIONS) {
        if (m_board.get(m_board.start_x + dir[0], m_board.start_y + dir[1])
                .state == CellState::FLOOR) {
            Route &r = current[current_length++];
            r.length = 2;
            r.x = m_board.start_x + dir[0];
            r.y = m_board.start_y + dir[1];
            r.direction = static_cast<Direction>(dir[2]);
            r.turns = r.direction == Direction::RIGHT ? 0 : 1;
            m_board.visit(r.x, r.y, 1 + 1000 * r.turns);
        }
    }

    while (current_length > 0) {
        next_length = 0;
        for (size_t i = 0; i < current_length; i++) {
            Route &r = current[i];
            const int32_t *d = deltas(r.direction);

            int32_t x = r.x + d[0];
            int32_t y = r.y + d[1];

            while (true) {
                if (m_board.get(x, y).state != CellState::FLOOR) {
                    break;
                }
                bool last =
                    m_board.get(x + d[0], y + d[1]).state == CellState::WALL &&
                    !(x == m_board.end_x && y == m_board.end_y);
                uint32_t score =
                    r.length + 1000 * (last ? r.turns + 1 : r.turns);

                if (m_board.get(x, y).score <= score) {
                    break;
                }

                m_board.visit(x, y, score);
                r.length++;

                uint32_t branch_score = r.length + (1000 * (r.turns + 1));

                if (m_board.get(x + d[2], y + d[3]).state == CellState::FLOOR &&
                    m_board.get(x + d[2], y + d[3]).score > branch_score) {
                    // JC_ASSERT(next_length < 128, "next_length < 128");
                    Route &next_r = next[next_length++];
                    next_r.length = r.length + 1;
                    next_r.x = x + d[2];
                    next_r.y = y + d[3];
                    next_r.direction = static_cast<Direction>(d[4]);
                    next_r.turns = r.turns + 1;
                    m_board.visit(next_r.x, next_r.y, branch_score);
                }
                if (m_board.get(x + d[5], y + d[6]).state == CellState::FLOOR &&
                    m_board.get(x + d[5], y + d[6]).score > branch_score) {
                    // JC_ASSERT(next_length < 128, "next_length < 128");
                    Route &next_r = next[next_length++];
                    next_r.length = r.length + 1;
                    next_r.x = x + d[5];
                    next_r.y = y + d[6];
                    next_r.direction = static_cast<Direction>(d[7]);
                    next_r.turns = r.turns + 1;
                    m_board.visit(next_r.x, next_r.y, branch_score);
                }

                x += d[0];
                y += d[1];
            }
        }
        // m_board.print();

        Route *temp = current;
        current = next;
        next = temp;

        current_length = next_length;
    }

    Cell &end = m_board.get(m_board.end_x, m_board.end_y);
    m_result.part1 = end.score;

    // //
    current_length = 0;

    m_board.cells[m_board.end_y][m_board.end_x].state = CellState::OPTIMAL;
    m_result.part2 = 1;

    for (const auto &dir : START_DIRECTIONS) {
        if (m_board.get(m_board.end_x + dir[0], m_board.end_y + dir[1]).score <
            m_result.part1) {
            Route &r = current[current_length++];
            r.x = m_board.end_x + dir[0];
            r.y = m_board.end_y + dir[1];
            r.direction = static_cast<Direction>(dir[2]);
            m_board.get(r.x, r.y).state = CellState::OPTIMAL;
            m_result.part2++;
        }
    }

    while (current_length > 0) {
        next_length = 0;
        for (size_t i = 0; i < current_length; i++) {
            Route &r = current[i];
            const int32_t *d = deltas(r.direction);

            int32_t x = r.x + d[0];
            int32_t y = r.y + d[1];

            uint32_t last_score = m_board.get(r.x, r.y).score + 1;

            while ((m_board.get(x, y).score < last_score ||
                    m_board.get(x, y).score + 2 ==
                        m_board.get(x - 2 * d[0], y - 2 * d[1]).score)
                   && m_board.get(x, y).state == CellState::FLOOR) {
                last_score = m_board.get(x, y).score;
                m_board.get(x, y).state = CellState::OPTIMAL;
                m_result.part2++;

                if (m_board.get(x + d[2], y + d[3]).score < last_score &&
                    m_board.get(x + d[2], y + d[3]).state !=
                        CellState::OPTIMAL) {
                    // JC_ASSERT(next_length < 128, "next_length < 128");
                    Route &next_r = next[next_length++];
                    next_r.x = x + d[2];
                    next_r.y = y + d[3];
                    next_r.direction = static_cast<Direction>(d[4]);
                    m_board.get(next_r.x, next_r.y).state = CellState::OPTIMAL;
                    m_result.part2++;
                }
                if (m_board.get(x + d[5], y + d[6]).score < last_score &&
                    m_board.get(x + d[5], y + d[6]).state !=
                        CellState::OPTIMAL) {
                    // JC_ASSERT(next_length < 128, "next_length < 128");
                    Route &next_r = next[next_length++];
                    next_r.x = x + d[5];
                    next_r.y = y + d[6];
                    next_r.direction = static_cast<Direction>(d[7]);
                    m_board.get(next_r.x, next_r.y).state = CellState::OPTIMAL;
                    m_result.part2++;
                }

                x += d[0];
                y += d[1];
            }
        }
        // m_board.print();

        Route *temp = current;
        current = next;
        next = temp;

        current_length = next_length;
    }
}
}  // namespace AOC
