#pragma once

#include "day.hpp"
namespace AOC {
class Day13 final : public LineByLineDay {
   public:
    Day13() : LineByLineDay(13) {}

   protected:
    void process_line(const std::string &line) override;
    Result expected() override { return {29201, 104140871044942}; }

   private:
    size_t m_line_n = 0;
    int64_t dx_1{}, dx_2{}, dy_1{}, dy_2{};
};
}  // namespace AOC
