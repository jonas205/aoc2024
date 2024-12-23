#pragma once

#include "day.hpp"
namespace AOC {

class Day22 final : public LineByLineDay {
   public:
    Day22() : LineByLineDay(22) {}

   protected:
    void process_line(const std::string &line) override;
    Result expected() override { return {12664695565, 1444}; }
    void post_processing() override;
   private:
    void crime_against_programing(uint64_t *numbers);
    void check_sequence(uint64_t *numbers, int32_t a, int32_t b, int32_t c,
                        int32_t d);

    uint64_t sum[19][19][19][19] = {{{{0}}}};
    uint64_t sum_get(int32_t a, int32_t b, int32_t c, int32_t d) {
        return sum[a + 9][b + 9][c + 9][d + 9];
    }
    void sum_add(int32_t a, int32_t b, int32_t c, int32_t d, uint64_t value) {
        sum[a + 9][b + 9][c + 9][d + 9] += value;
    }
};
}  // namespace AOC
