#include "day_21.hpp"
#include "day.hpp"

#include <cstdlib>

namespace AOC {

uint64_t Day21::DirLayer::press_next_or_return(const char c) {
    if (next_layer == nullptr) {
        return 1;
    }
    return next_layer->press(c);
}

uint64_t Day21::DirLayer::press(const char c) {
    Point p = Point::dir_from_char(c);

    int32_t &x = current.x;
    int32_t &y = current.y;

    const size_t id = cache_id(p);
    if (cache[id] != 0) {
        x = p.x;
        y = p.y;
        return cache[id];
    }

    uint64_t erg = 0;

    while ((y == 1 || p.x > 0) && p.x < x) {
        erg += press_next_or_return('<');
        x--;
    }
    while (x > 0 && p.y < y) {
        erg += press_next_or_return('^');
        y--;
    }
    while (p.y > y) {
        erg += press_next_or_return('v');
        y++;
    }
    while (p.x < x) {
        erg += press_next_or_return('<');
        x--;
    }
    while (p.x > x) {
        erg += press_next_or_return('>');
        x++;
    }
    while (p.y < y) {
        erg += press_next_or_return('^');
        y--;
    }
    erg += press_next_or_return('A');

    return cache[id] = erg;
}

uint64_t Day21::NumericLayer::press(const char c) {
    Point p = Point::numeric_from_char(c);

    int32_t &x = current.x;
    int32_t &y = current.y;

    uint64_t erg = 0;

    if (y < 3 || (y == 3 && p.x > 0)) {
        while (p.x < x) {
            erg += next_layer.press('<');
            x--;
        }
    }
    if (x > 0 || (x == 0 && p.x < 3)) {
        while (p.y > y) {
            erg += next_layer.press('v');
            y++;
        }
    }
    while (p.y < y) {
        erg += next_layer.press('^');
        y--;
    }
    while (p.x > x) {
        erg += next_layer.press('>');
        x++;
    }
    while (p.x < x) {
        erg += next_layer.press('<');
        x--;
    }
    while (p.y > y) {
        erg += next_layer.press('v');
        y++;
    }

    erg += next_layer.press('A');

    return erg;
}

void Day21::process_line(const std::string &line) {
    uint64_t erg_1 = 0;
    uint64_t erg_2 = 0;

    for (size_t i = 0; i < line.length() - 1; i++) {
        erg_1 += m_layer_p1.press(line[i]);
        erg_2 += m_layer_p2.press(line[i]);
    }

    uint64_t n = 0;
    size_t i = 0;
    while (line[i] >= '0' && line[i] <= '9') {
        n = n * 10 + line[i] - '0';
        i++;
    }

    m_result.part1 += erg_1 * n;
    m_result.part2 += erg_2 * n;
}

}  // namespace AOC
