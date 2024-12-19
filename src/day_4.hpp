#pragma once

#include "day.hpp"
namespace AOC {

class Day4 final : public LineByLineDay {
   public:
    Day4() : LineByLineDay(4) {}

    struct Grid {
        char grid[200][200]{};
        size_t width = 0, height = 0;

        [[nodiscard]] char get(const size_t x, const size_t y) const {
            if (x >= width || y >= height) {
                return '.';
            }
            return grid[x][y];
        }
    };

   protected:
    void process_line(const std::string &line) override;
    void post_processing() override;
    Result expected() override { return {2573, 1850}; }

   private:
    Grid m_grid;
};
}  // namespace AOC
