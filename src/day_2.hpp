#pragma once

#include "day.hpp"
namespace AOC {
class Day2 final : public LineByLineDay {
   public:
    Day2() : LineByLineDay(2) {}

   protected:
    void process_line(const std::string &line) override;
    Result expected() override { return {269, 337}; }
};
}  // namespace AOC
