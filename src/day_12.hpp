#pragma once

#include "day.hpp"
namespace AOC {
class Day12 final : public LineByLineDay {
   public:
    Day12() : LineByLineDay(12) {}

   protected:
    void process_line(const std::string &line) override;
    void post_processing() override;
    Result expected() override { return {1361494, 830516}; }

   private:
    struct Cell {
        char type{};
        bool checked = false;
    };

    struct {
        Cell cell[150][150];
        size_t width = 0, height = 0;
        Cell dummy_cell = {'.'};
    } m_grid;

    struct Region {
        uint64_t area;
        uint64_t perimeter;
        uint64_t sides;
    };

    Region find_region(int32_t x, int32_t y);

    Cell &cell_at(const int32_t x, const int32_t y) {
        if (x < 0 || y < 0 || static_cast<size_t>(x) >= m_grid.width || static_cast<size_t>(y) >= m_grid.height) {
            return m_grid.dummy_cell;
        }
        return m_grid.cell[y][x];
    }
};
}  // namespace AOC
