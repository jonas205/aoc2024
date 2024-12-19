#pragma once

#include "day.hpp"
namespace AOC {
class Day14 final : public LineByLineDay {
   public:
    Day14() : LineByLineDay(14) {}

   protected:
    void process_line(const std::string &line) override;
    Result expected() override { return {226236192, 8168}; }
    void post_processing() override;

   private:
    struct {
        uint32_t ul = 0, ur = 0, dl = 0, dr = 0;
    } m_quadrants;
    struct Robot {
        int32_t start_x = 0, start_y = 0, dx = 0, dy = 0;
    };
    Robot m_robots[500];
    size_t m_robots_n = 0;

    bool check_iteration(int32_t i);
    void draw(int32_t i);
};
}  // namespace AOC
