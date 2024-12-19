#pragma once

#include "day.hpp"
namespace AOC {
class Day1 final : public LineByLineDay {
   public:
    Day1() : LineByLineDay(1) {}

   protected:
    void process_line(const std::string &line) override;
    void post_processing() override;
    Result expected() override { return {2264607, 19457120}; }

   private:
    std::vector<uint32_t> list_0, list_1;
};
}  // namespace AOC
