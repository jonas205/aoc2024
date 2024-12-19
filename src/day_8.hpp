#pragma once

#include "day.hpp"
namespace AOC {

class Day8 final : public LineByLineDay {
   public:
    Day8() : LineByLineDay(8) {}

   protected:
    void process_line(const std::string &line) override;
    Result expected() override { return {269, 949}; }

   private:
    class Cell {
       public:
        uint8_t mark_part_1() {
            if (value & CELL_P1) {
                return 0;
            }
                value |= CELL_P1;
                return 1;

        }
        uint8_t mark_part_2() {
            if (value & CELL_P2) {
                return 0;
            }
                value |= CELL_P2;
                return 1;

        }

       private:
        static constexpr uint8_t CELL_EMPTY = 0x00;
        static constexpr uint8_t CELL_P1 = 0x01;
        static constexpr uint8_t CELL_P2 = 0x10;
        uint8_t value = 0;
    };

    struct {
        Cell grid[50][50];
        size_t width = 0, height = 0;
    } m_grid;

    struct Point {
        int32_t x, y;
    };

    Point m_points[26 + 26 + 10][50]{};
    size_t m_points_length[26 + 26 + 10]{};

    static size_t char_to_index(const char c) {
        if (c >= 'a') {
            return c - 'a';
        }
        if (c >= 'A') {
            return 26 + c - 'A';
        }

            return 2 * 26 + c - '0';

    }
};
}  // namespace AOC
