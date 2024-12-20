
#pragma once

#include "day.hpp"
namespace AOC {

class Day20 final : public LineByLineDay {
   public:
    Day20() : LineByLineDay(20) {}

   protected:
    void process_line(const std::string &line) override;
    Result expected() override { return {1445, 1008040}; }
    void post_processing() override;

   private:
    enum struct CellState : uint8_t {
        Wall,
        Empty,
    };

    struct Cell {
        CellState state{};
        uint32_t value = UINT32_MAX;
    };

    struct Point {
        int32_t x, y;
        uint32_t value;
    };

    struct Board {
        Cell cells[150][150]{};
        size_t width = 0, height = 0;
        int32_t start_x{}, start_y{}, end_x{}, end_y{};

        Point points[10000];
        size_t points_length = 0;

        uint32_t last_value{};

        void print();
        void walk_board(Day20 &day,
                        void (*callback)(Board &, Day20 &, int32_t, int32_t));
        uint32_t get(int32_t x, int32_t y) {
            if (x < 0 || y < 0 || x >= static_cast<int32_t>(width) ||
                y >= static_cast<int32_t>(height)) {
                return UINT32_MAX;
            }
            return cells[y][x].value;
        }
    };

    Board m_board{};

    friend void day20_value_assigner(Day20::Board &board, Day20 &day, int32_t x,
                                     int32_t y);
    friend void day20_cheater(Day20::Board &board, Day20 &day, int32_t x,
                              int32_t y);
};

}  // namespace AOC
