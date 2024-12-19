#include "day_3.hpp"
#include <cstdio>

namespace AOC {

static void detect_mul(const std::string &line, size_t &i, Result &result,
                       const bool enabled) {
    const size_t length = line.length();
    i++;
    if (i == length || line[i] != 'u') {
        return;
    }
    i++;
    if (i == length || line[i] != 'l') {
        return;
    }
    i++;
    if (i == length || line[i] != '(') {
        return;
    }
    i++;
    int x;
    int success = sscanf(line.c_str() + i, "%d", &x);
    if (success == 0 || x > 999 || x <= 0) {
        return;
    }
    i++;
    if (x > 9) {
        i++;
    }
    if (x > 99) {
        i++;
    }
    if (i == length || line[i] != ',') {
        return;
    }
    i++;
    int y;
    success = sscanf(line.c_str() + i, "%d", &y);
    if (success == 0 || y > 999 || y <= 0) {
        return;
    }
    i++;
    if (y > 9) {
        i++;
    }
    if (y > 99) {
        i++;
    }
    if (i == length || line[i] != ')') {
        return;
    }
    result.part1 += x * y;
    if (enabled) {
        result.part2 += x * y;
    }
}

static void detect_dos(const std::string &line, size_t &i, bool &enabled) {
    const size_t length = line.length();
    i++;
    if (i == length || line[i] != 'o') {
        return;
    }
    i++;
    if (i + 1 < length && line[i] == '(' && line[i + 1] == ')') {
        enabled = true;
        return;
    }
    if (i == length || line[i] != 'n') {
        return;
    }
    i++;
    if (i == length || line[i] != '\'') {
        return;
    }
    i++;
    if (i == length || line[i] != 't') {
        return;
    }
    i++;
    if (i == length || line[i] != '(') {
        return;
    }
    i++;
    if (i == length || line[i] != ')') {
        return;
    }
    enabled = false;
}

void Day3::process_line(const std::string &line) {
    for (size_t i = 0; i < line.length(); i++) {
        if (line[i] == 'm') {
            detect_mul(line, i, m_result, m_enabled);
        }
        if (line[i] == 'd') {
            detect_dos(line, i, m_enabled);
        }
    }
}

}  // namespace AOC
