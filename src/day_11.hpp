#pragma once

#include "day.hpp"
namespace AOC {
class Day11 final : public Day {
   public:
    Day11() : Day(11) {}

   protected:
    void calculate(const std::string &content) override;
    Result expected() override { return {216996, 257335372288947}; }
};
}  // namespace AOC
