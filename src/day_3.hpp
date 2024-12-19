
#pragma once

#include "day.hpp"
namespace AOC {
class Day3 final : public LineByLineDay {
   public:
    Day3() : LineByLineDay(3) {}

   protected:
    void process_line(const std::string &line) override;
    Result expected() override { return {174103751, 100411201}; }

   private:
    bool m_enabled = true;
};
}  // namespace AOC
