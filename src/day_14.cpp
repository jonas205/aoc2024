#include "day_14.hpp"

#include <cassert>
namespace AOC {

static constexpr int32_t S_WIDTH = 101, S_HEIGHT = 103;

static void parse_input(const std::string &line, int32_t &start_x,
                        int32_t &start_y, int32_t &dx, int32_t &dy) {
    size_t i = 2;

    start_x = 0;
    while (line[i] >= '0' && line[i] <= '9') {
        start_x = 10 * start_x + line[i] - '0';
        i++;
    }
    i += 1;

    start_y = 0;
    while (line[i] >= '0' && line[i] <= '9') {
        start_y = 10 * start_y + line[i] - '0';
        i++;
    }
    i += 3;

    dx = 0;
    bool dx_negative = false;
    if (line[i] == '-') {
        dx_negative = true;
        i++;
    }
    while (line[i] >= '0' && line[i] <= '9') {
        dx = 10 * dx + line[i] - '0';
        i++;
    }
    i += 1;
    if (dx_negative) {
        dx = -dx;
    }

    dy = 0;
    bool dy_negative = false;
    if (line[i] == '-') {
        dy_negative = true;
        i++;
    }
    while (line[i] >= '0' && line[i] <= '9') {
        dy = 10 * dy + line[i] - '0';
        i++;
    }
    if (dy_negative) {
        dy = -dy;
    }
}

void Day14::process_line(const std::string &line) {
    static constexpr int32_t ITERATIONS = 100;
    Robot &bot = m_robots[m_robots_n++];
    int32_t &start_x = bot.start_x;
    int32_t &start_y = bot.start_y;
    int32_t &dx = bot.dx;
    int32_t &dy = bot.dy;

    parse_input(line, start_x, start_y, dx, dy);

    int32_t x = (start_x + ITERATIONS * dx) % S_WIDTH;
    int32_t y = (start_y + ITERATIONS * dy) % S_HEIGHT;

    if (x < 0) {
        x += S_WIDTH;
    }
    if (y < 0) {
        y += S_HEIGHT;
    }

    if (x < S_WIDTH / 2 && y < S_HEIGHT / 2) {
        m_quadrants.ul++;
    } else if (y < S_HEIGHT / 2 && x > S_WIDTH / 2) {
        m_quadrants.ur++;
    } else if (x < S_WIDTH / 2 && y > S_HEIGHT / 2) {
        m_quadrants.dl++;
    } else if (x > S_WIDTH / 2 && y > S_HEIGHT / 2) {
        m_quadrants.dr++;
    }
    // JC_TRACE << "Start: (" << start_x << ", " << start_y << ") Deltas: (" << dx
    //          << ", " << dy << ") Pos: (" << x << ", " << y << ")\n";
}

bool Day14::check_iteration(int32_t i) {
    static constexpr int32_t DISTANCE = 20;
    static constexpr size_t SKIP = 1;

    int32_t avg_x = 0, avg_y = 0;
    int32_t points[500][2];

    for (size_t j = 0; j < m_robots_n; j += SKIP) {
        auto &[start_x, start_y, dx, dy] = m_robots[j];
        int32_t x = 0;
        int32_t y = 0;

        x = (start_x + i * dx) % S_WIDTH;
        y = (start_y + i * dy) % S_HEIGHT;

        if (x < 0) {
            x += S_WIDTH;
        }
        if (y < 0) {
            y += S_HEIGHT;
        }
        avg_x += x;
        avg_y += y;
        points[j][0] = x;
        points[j][1] = y;
    }

    avg_x /= static_cast<int32_t>(m_robots_n);
    avg_y /= static_cast<int32_t>(m_robots_n);

    int32_t avg_dist_x = 0;
    int32_t avg_dist_y = 0;

    for (size_t j = 0; j < m_robots_n; j += SKIP) {
        avg_dist_x += std::abs(points[j][0] - avg_x);
        avg_dist_y += std::abs(points[j][1] - avg_y);
    }

    avg_dist_x /= static_cast<int32_t>(m_robots_n);
    avg_dist_y /= static_cast<int32_t>(m_robots_n);

    // JC_TRACE << "Iteration: " << i << " Avg dist x: " << avg_dist_x
    //          << " Avg dist y: " << avg_dist_y << '\n';
    return avg_dist_x < DISTANCE && avg_dist_y < DISTANCE;
}

void Day14::draw(int32_t i) {
    JC_PROFILE_FUNCTION();
    bool grid[S_HEIGHT][S_WIDTH] = {{false}};
    for (size_t j = 0; j < m_robots_n; j++) {
        auto &[start_x, start_y, dx, dy] = m_robots[j];
        int32_t x = 0;
        int32_t y = 0;

        x = (start_x + i * dx) % S_WIDTH;
        y = (start_y + i * dy) % S_HEIGHT;

        if (x < 0) {
            x += S_WIDTH;
        }
        if (y < 0) {
            y += S_HEIGHT;
        }

        grid[y][x] = true;
    }

    for (const auto & y : grid) {
        for (const bool x : y) {
            if (x) {
                std::cout << '#';
            } else {
                std::cout << '.';
            }
        }
        std::cout << '\n';
    }
}

void Day14::post_processing() {
    // JC_TRACE << "Quadrants: UL: " << m_quadrants.ul << " UR: " << m_quadrants.ur
    //          << " DL: " << m_quadrants.dl << " DR: " << m_quadrants.dr << '\n';
    m_result.part1 =
        m_quadrants.ul * m_quadrants.ur * m_quadrants.dl * m_quadrants.dr;

    int32_t result = -1;

#pragma omp parallel
    {
#pragma omp for
        for (int32_t i = 1; i < 10000; i++) {
            if (result == -1 && check_iteration(i)) {
#pragma omp critical
                {
                    if (result == -1) {
                        result = i;
                    }
                }
            }
        }
    }

    m_result.part2 = result;
}

}  // namespace AOC
