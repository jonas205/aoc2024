#pragma once

#include "day.hpp"
namespace AOC {

class Day18 final : public LineByLineDay {
   public:
    Day18() : LineByLineDay(18) {}

   protected:
    void process_line(const std::string &line) override;
    Result expected() override { return {296, 280044}; }
    void post_processing() override;

   private:
    // static constexpr size_t S_SIZE = 6 + 1;
    // static constexpr size_t S_LIMIT = 12;
    //
    static constexpr size_t S_SIZE = 70 + 1;
    static constexpr size_t S_LIMIT = 1024;

    struct Point {
        size_t x, y;
    };

    Point m_points[4096];
    size_t m_points_size = 0;

    enum class CellType : uint8_t {
        NEW,
        WALL,
    };

    struct Cell {
        CellType type = CellType::NEW;
        uint32_t cost = UINT32_MAX;
    };

    struct Board {
        Cell cells[S_SIZE][S_SIZE]{};
        Cell dummy_cell = {CellType::WALL, UINT32_MAX};

        Board copy() const;
        void print();
        bool solve(uint64_t &result);

        Cell &get(size_t x, size_t y) {
            if (x >= S_SIZE || y >= S_SIZE) {
                return dummy_cell;
            }
            return cells[y][x];
        }
    };

    Board m_board;

    size_t m_fallen = 0;
};
}  // namespace AOC
