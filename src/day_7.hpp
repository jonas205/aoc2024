
#pragma once

#include "day.hpp"
namespace AOC {


class Day7 final : public LineByLineDay {
   public:
    Day7() : LineByLineDay(7) {}

   protected:
    void process_line(const std::string &line) override;
    Result expected() override { return {7710205485870, 20928985450275}; }

   private:
    bool part_1(uint64_t target, uint64_t rem, size_t i);
    bool part_2(uint64_t target, uint64_t rem, size_t i);

    uint32_t m_line[32]{};
    size_t m_line_length{};
};
}  // namespace AOC
