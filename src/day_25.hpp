#pragma once

#include "day.hpp"
namespace AOC {

class Day25 final : public LineByLineDay {
   public:
    Day25() : LineByLineDay(25) {}

   protected:
    void process_line(const std::string &line) override;
    Result expected() override { return {3065, 0}; }
    void post_processing() override;

   private:
    struct Common {
        uint8_t value[5];

        void init(uint8_t *v);
        void print();

       protected:
    };
    struct Key : public Common {};
    struct Lock : public Common {};

    static bool match(const Key &key, const Lock &lock);

    uint8_t m_parsing[5] = {0};
    size_t m_line_count = 0;

    static constexpr size_t MAX_SIZE = 256;

    Lock m_locks[MAX_SIZE];
    size_t m_lock_count = 0;

    Key m_keys[MAX_SIZE];
    size_t m_key_count = 0;
};
}  // namespace AOC
