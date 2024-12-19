
#pragma once

#include "day.hpp"
namespace AOC {
class Day10 final : public LineByLineDay {
   public:
    Day10() : LineByLineDay(10) {}

   protected:
    void process_line(const std::string &line) override;
    Result expected() override { return {538, 1110}; }
    void post_processing() override;

   private:
    void next(size_t x, size_t y, bool fresh = true);

    struct Cell {
        int8_t value;
        int8_t last_seen = 0;
    };

    struct {
        Cell cells[50][50]{};
        size_t width = 0, height = 0;
        int8_t current_0 = 0;
    } m_grid;

     [[nodiscard]] int8_t get(const size_t x, const size_t y) const {
        if (x >= m_grid.width || y >= m_grid.height) {
            return -1;
        }
        return m_grid.cells[y][x].value;
    }
};
}  // namespace AOC
