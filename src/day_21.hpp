#pragma once

#include <memory>
#include "day.hpp"
namespace AOC {

class Day21 final : public LineByLineDay {
   public:
    Day21() : LineByLineDay(21) {}

   protected:
    void process_line(const std::string &line) override;
    Result expected() override { return {238078, 293919502998014}; }
    // void post_processing() override;

   private:
    struct Point {
        int32_t x, y;

        static Point dir_from_char(const char c) {
            switch (c) {
                case '^':
                    return {1, 0};
                case '<':
                    return {0, 1};
                case 'v':
                    return {1, 1};
                case '>':
                    return {2, 1};
                case 'A':
                    return {2, 0};
                default:
                    die("Invalid numeric char: " << c);
            }
        }

        static Point numeric_from_char(const char c) {
            switch (c) {
                case '7':
                    return {0, 0};
                case '8':
                    return {1, 0};
                case '9':
                    return {2, 0};
                case '4':
                    return {0, 1};
                case '5':
                    return {1, 1};
                case '6':
                    return {2, 1};
                case '1':
                    return {0, 2};
                case '2':
                    return {1, 2};
                case '3':
                    return {2, 2};
                case '0':
                    return {1, 3};
                case 'A':
                    return {2, 3};
                default:
                    die("Invalid numeric char: " << c);
            }
        }
    };

    class DirLayer {
       public:
        explicit DirLayer(const size_t count)
            : next_layer(count > 1 ? std::make_unique<DirLayer>(count - 1)
                                   : nullptr) {}

        uint64_t press(char c);
        uint64_t press_next_or_return(char c);

       private:
        std::unique_ptr<DirLayer> next_layer;
        Point current = Point::dir_from_char('A');
        uint64_t cache[64]{0};
        [[nodiscard]] size_t cache_id(const Point &target) const {
            return (current.x) | (current.y << 2) | (target.x << 3) |
                   (target.y << 5);
        }
    };

    class NumericLayer {
       public:
        explicit NumericLayer(const size_t count) : next_layer(count) {}
        uint64_t press(char c);

       private:
        DirLayer next_layer;
        Point current = Point::numeric_from_char('A');
    };

    NumericLayer m_layer_p1{2};
    NumericLayer m_layer_p2{25};
};
}  // namespace AOC
