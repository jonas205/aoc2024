#pragma once

#include "day.hpp"
namespace AOC {

class Day16 final : public LineByLineDay {
   public:
    Day16() : LineByLineDay(16) {}

   protected:
    void process_line(const std::string &line) override;
    Result expected() override { return {73432, 496}; }
    void post_processing() override;

   private:
    enum class CellState {
        WALL,
        OPTIMAL,
        FLOOR,
    };
    struct Cell {
        CellState state{};
        uint32_t score = UINT32_MAX;
    };
    struct {
        Cell cells[150][150]{};

        size_t width = 0, height = 0;
        int32_t start_x{}, start_y{};
        int32_t end_x{}, end_y{};

        inline Cell &get(size_t x, size_t y) {
            // Walls sourround the board -> no need to check for out of bounds
            return cells[y][x];
        }
        inline void visit(size_t x, size_t y, uint32_t score) {
            cells[y][x].score = score;
        }
        inline void print() {
            std::cout << '+';
            for (size_t x = 0; x < width; ++x) {
                std::cout << x % 10;
            }
            std::cout << '\n';
            for (size_t y = 0; y < height; ++y) {
                std::cout << y % 10;
                for (size_t x = 0; x < width; ++x) {
                    switch (cells[y][x].state) {
                        case CellState::WALL:
                            std::cout << '#';
                            break;
                        case CellState::FLOOR:
                            if (cells[y][x].score == UINT32_MAX) {
                                std::cout << ' ';
                            } else {
                                std::cout << '.';
                            }
                            break;
                        case CellState::OPTIMAL:
                            std::cout << 'O';
                            break;
                    }
                }
                std::cout << '\n';
            }
        }
    } m_board;
};
}  // namespace AOC
