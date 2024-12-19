#pragma once

#include "day.hpp"
namespace AOC {
class Day9 final : public Day {
   public:
    Day9() : Day(9) {}

   protected:
    void calculate(const std::string &content) override;
    Result expected() override { return {6283404590840, 6304576012713}; }
};
}  // namespace AOC
