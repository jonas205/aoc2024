#include "day.hpp"
#include "day_1.hpp"
#include "day_11.hpp"
#include "day_12.hpp"
#include "day_13.hpp"
#include "day_14.hpp"
#include "day_15.hpp"
#include "day_16.hpp"
#include "day_18.hpp"
#include "day_2.hpp"
#include "day_3.hpp"
#include "day_4.hpp"
#include "day_5.hpp"
#include "day_6.hpp"
#include "day_7.hpp"
#include "day_8.hpp"
#include "day_9.hpp"
#include "day_10.hpp"
#include "day_17.hpp"
#include <chrono>

namespace AOC {
void run_all() {
    JCPP::LOG::Logger logger("aoc.log");
    Day *days[] = {
        new Day1(),
        new Day2(),
        new Day3(),
        new Day4(),
        new Day5(),
        new Day6(),
        new Day7(),
        new Day8(),
        new Day9(),
        new Day10(),
        new Day11(),
        new Day12(),
        new Day13(),
        new Day14(),
        new Day15(),
        new Day16(),
        new Day17(),
        new Day18(),
    };

    JC_PROFILE_BEGIN_SESSION("aoc", "aoc.json");
    {
        const auto start = std::chrono::high_resolution_clock::now();
        for (auto &day : days) {
            day->run();
        }
        const auto end = std::chrono::high_resolution_clock::now();

        const auto elapsed =
        std::chrono::time_point_cast<std::chrono::microseconds>(end)
            .time_since_epoch() -
        std::chrono::time_point_cast<std::chrono::microseconds>(start)
            .time_since_epoch();

        uint32_t us = elapsed.count();
        uint32_t ms = us / 1000;
        const uint32_t s = ms / 1000;

        ms %= 1000;
        us %= 1000;

        info("Elapsed time: " << s << "s " << ms << "ms " << us << "us");
    }
    JC_PROFILE_END_SESSION();

    for (auto &day : days) {
        delete day;
    }
}
}  // namespace AOC
int main() {
    AOC::run_all();
}