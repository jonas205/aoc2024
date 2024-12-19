#pragma once

#include "day.hpp"
namespace AOC {

class Day17 final : public StringDay {
   public:
    Day17() : StringDay(17) {}
    void calculate(const std::string &file_content) override;

   protected:
    StringResult string_expected() override {
        return {"2,7,4,7,2,1,7,5,1", "37221274271220"};
    }

   private:
    void parse_input(const std::string &file_content);
    void execute();
    bool next_digit(int64_t a, size_t pos);

    struct {
        int64_t a{}, b{}, c{};
        int8_t ip = 0;
    } m_cpu;

    // b = a % 8
    // b = b ^ 2
    // c = a / (2 ** b)
    // b = b ^ c
    // b = b ^ 3
    // print(b % 8)
    // a = a / 8
    // if (a != 0) goto 0

    enum class Instruction : uint8_t {
        adv = 0,
        bxl = 1,
        bst = 2,
        jnz = 3,
        bxc = 4,
        out = 5,
        bdv = 6,
        cdv = 7,
    };

    [[nodiscard]] int64_t combo() const {
        switch (m_program[m_cpu.ip + 1]) {
            case 4:
                return m_cpu.a;
            case 5:
                return m_cpu.b;
            case 6:
                return m_cpu.c;
            default:
                return m_program[m_cpu.ip + 1];
        }
    }

    [[nodiscard]] int64_t literal() const { return m_program[m_cpu.ip + 1]; }

    Instruction get_instruction() {
        return static_cast<Instruction>(m_program[m_cpu.ip]);
    }

    int8_t m_program[32]{};
    size_t m_program_size = 0;
    char m_output[256]{};
    size_t m_output_size = 0;
};
}  // namespace AOC
