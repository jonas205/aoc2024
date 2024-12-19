#pragma once

#include "day.hpp"
namespace AOC {

class Day15 final : public LineByLineDay {
   public:
    Day15() : LineByLineDay(15) {}

   protected:
    void process_line(const std::string &line) override;
    Result expected() override { return {1426855, 1404917}; }
    void post_processing() override;

   private:
    enum class Cell : uint8_t {
        EMPTY,
        BOX,
        WALL,

        BOX_LEFT,
        BOX_RIGHT,
    };
    enum class Move : uint8_t {
        LEFT = 0,
        RIGHT = 1,
        UP = 2,
        DOWN = 3,
    };

    bool m_board_parsing = true;

    struct {
        Cell cells[50][50]{};
        size_t width = 0, height = 0;
        int32_t x{}, y{};
    } m_board;
    struct {
        Cell cells[50][100]{};
        size_t width = 0, height = 0;
        int32_t x{}, y{};
    } m_board_2;

    void print_1() const;
    void print_2() const;
    void move_1(Move move);
    void move_2(Move move);
    bool can_move_box(Move move, int32_t x, int32_t y);
    void move_box(Move move, int32_t x, int32_t y);
};
}  // namespace AOC
