#include "day_13.hpp"
#include <cmath>
#include <numeric>
namespace AOC {

struct Day13Number {
    int64_t n, dividend, divisor;

    Day13Number(const int64_t value) : n(value), dividend(0), divisor(1) {}
    Day13Number(const int64_t dividend, const int64_t divisor)
        : n(dividend / divisor),
          dividend(dividend % divisor),
          divisor(divisor) {}
    Day13Number(const int64_t n, const int64_t dividend, const int64_t divisor)
        : n(n), dividend(dividend), divisor(divisor) {}

    [[nodiscard]] bool is_zero() const { return dividend == 0 && n == 0; }
    [[nodiscard]] bool is_one() const { return n == 1 || (n == 0 && dividend == divisor); }

    void operator/=(const Day13Number &other) {
        const int64_t p = (n * divisor + dividend) * other.divisor;
        const int64_t q = (other.n * other.divisor + other.dividend) * divisor;

        dividend = p;
        divisor = q;
        n = 0;
    }

    Day13Number operator+(const Day13Number &other) const {
        return { n + other.n,
                           dividend * other.divisor + other.dividend * divisor,
                           divisor * other.divisor};
    }

    Day13Number operator*(const Day13Number &other) const {
        return Day13Number((n * divisor + dividend) *
                               (other.n * other.divisor + other.dividend),
                           divisor * other.divisor)
            .minimized();
    }
    void operator-=(const Day13Number &other) {
        n -= other.n;
        int64_t p = dividend * other.divisor - other.dividend * divisor;
        int64_t q = divisor * other.divisor;

        dividend = p;
        divisor = q;
    }

    [[nodiscard]] bool is_whole() const { return dividend % divisor == 0; }
    [[nodiscard]] uint64_t whole() const { return n + dividend / divisor; }

   private:
    [[nodiscard]] Day13Number minimized() const {
        int64_t gcd = std::gcd(dividend, divisor);
        int64_t p = dividend / gcd;
        int64_t q = divisor / gcd;
        if (p < 0 && q < 0) {
            p = -p;
            q = -q;
        }
        int64_t n = this->n + p / q;
        p = p % q;
        return { n,  p, q};
    }
};

struct Day13Result {
    uint64_t part1 = 0, part2 = 0;
};

static Day13Result solve(const int64_t p_dx_1, const int64_t p_dx_2,
                         const int64_t p_dy_1, const int64_t p_dy_2,
                         const int64_t p_x, const int64_t p_y) {
    static constexpr int64_t part_2_delta = 10000000000000;
    Day13Number dx_1 = p_dx_1, dx_2 = p_dx_2, dy_1 = p_dy_1, dy_2 = p_dy_2,
                x = p_x, y = p_y, x2 = p_x + part_2_delta,
                y2 = p_y + part_2_delta;

    if (!dx_1.is_one()) {
        dx_2 /= dx_1;
        x /= dx_1;
        x2 /= dx_1;
    }

    if (!dy_1.is_zero()) {
        dy_2 -= dy_1 * dx_2;
        y -= dy_1 * x;
        y2 -= dy_1 * x2;
    }

    if (!dy_2.is_one()) {
        y /= dy_2;
        y2 /= dy_2;
    }

    if (!dx_2.is_zero()) {
        x -= dx_2 * y;
        x2 -= dx_2 * y2;
    }

    Day13Result erg;
    if (x.is_whole() && y.is_whole()) {
        erg.part1 = 3 * x.whole() + y.whole();
    }
    if (x2.is_whole() && y2.is_whole()) {
        erg.part2 = 3 * x2.whole() + y2.whole();
    }

    return erg;
}

void Day13::process_line(const std::string &line) {
    size_t i = 0;
    switch (m_line_n) {
        case 0:
        case 1: {
            for (; line[i] != '+'; i++)
                ;
            i++;
            int32_t x = 0, y = 0;
            while (line[i] >= '0' && line[i] <= '9') {
                x = x * 10 + line[i] - '0';
                i++;
            }
            i += 4;
            while (line[i] >= '0' && line[i] <= '9') {
                y = y * 10 + line[i] - '0';
                i++;
            }
            if (m_line_n == 0) {
                dx_1 = x;
                dy_1 = y;
            } else {
                dx_2 = x;
                dy_2 = y;
            }
            break;
        }
        case 2: {
            for (; line[i] != '='; i++)
                ;
            i++;
            uint32_t x = 0, y = 0;
            while (line[i] >= '0' && line[i] <= '9') {
                x = x * 10 + line[i] - '0';
                i++;
            }
            i += 4;
            while (line[i] >= '0' && line[i] <= '9') {
                y = y * 10 + line[i] - '0';
                i++;
            }

            Day13Result res = solve(dx_1, dx_2, dy_1, dy_2, x, y);
            m_result.part1 += res.part1;
            m_result.part2 += res.part2;

            break;
        }
        case 3: {
            break;
        }
        default:;
    }

    m_line_n = (m_line_n + 1) % 4;
}

}  // namespace AOC
