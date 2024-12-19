#pragma once

#include "day.hpp"
namespace AOC {
class Day5 final : public LineByLineDay {
   public:
    Day5() : LineByLineDay(5), m_sorter(&m_order) {}

   protected:
    void process_line(const std::string &line) override;
    Result expected() override { return {5509, 4407}; }

   private:
    struct Order {
        uint32_t order[100][25] = {{0}};
    };

    struct Sorter {
        explicit Sorter(Order *order) : order(order) {}

        bool operator()(const uint32_t a, const uint32_t b) const {
            for (uint32_t i = 1; i <= order->order[a][0]; i++) {
                if (order->order[a][i] == b) {
                    return true;
                }
            }
            return false;
        }

        Order *order;
    };

    void map_line(const std::string &line);
    void check_line();

    Sorter m_sorter;

    bool m_mapping_mode = true;
    uint32_t m_line[50]{};
    Order m_order{};
    uint32_t m_line_length{};
};
}  // namespace AOC
