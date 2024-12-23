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
    int32_t sum_arr[19][19][19][19] = {{{{0}}}};
    uint64_t last_seen_arr[19][19][19][19] = {{{{0}}}};

    int32_t &sum(int32_t a, int32_t b, int32_t c, int32_t d) {
        return sum_arr[a + 9][b + 9][c + 9][d + 9];
    }
    uint64_t &last_seen(int32_t a, int32_t b, int32_t c, int32_t d) {
        return last_seen_arr[a + 9][b + 9][c + 9][d + 9];
    }
};
}  // namespace AOC
