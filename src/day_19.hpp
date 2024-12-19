#pragma once

#include "day.hpp"
namespace AOC {

class Day19 final : public LineByLineDay {
   public:
    Day19() : LineByLineDay(19) {}

   protected:
    void process_line(const std::string &line) override;
    Result expected() override { return {269, 758839075658876}; }
    void post_processing() override;

   private:
    void map_towels(const std::string &line);
    size_t is_possible(const std::string &line, size_t pos, size_t *line_cache);

    enum class Color : uint8_t {
        white = 'w',
        blue = 'u',
        black = 'b',
        red = 'r',
        green = 'g',
    };

    static Color char_to_color(char c) { return static_cast<Color>(c); }
    static char color_to_char(Color c) { return static_cast<char>(c); }

    Color m_towel[512][16]{};
    size_t m_towel_size = 0;
    size_t m_towel_sizes[512] = {0};

    std::string m_lines[512]{};
    size_t m_lines_count = 0;

    size_t m_line_count = 0;
};
}  // namespace AOC
