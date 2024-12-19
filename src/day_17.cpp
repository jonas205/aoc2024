#include "day_17.hpp"

namespace AOC {

void Day17::parse_input(const std::string &file_content) {
    size_t i = 12;
    m_cpu.a = 0;
    while (file_content[i] >= '0' && file_content[i] <= '9') {
        m_cpu.a = m_cpu.a * 10 + file_content[i] - '0';
        i++;
    }

    i += 13;
    m_cpu.b = 0;
    while (file_content[i] >= '0' && file_content[i] <= '9') {
        m_cpu.b = m_cpu.b * 10 + file_content[i] - '0';
        i++;
    }

    i += 13;
    m_cpu.c = 0;
    while (file_content[i] >= '0' && file_content[i] <= '9') {
        m_cpu.c = m_cpu.c * 10 + file_content[i] - '0';
        i++;
    }

    i += 11;

    while (file_content[i] != '\n') {
        m_program[m_program_size++] = file_content[i] - '0';
        i += 2;
    }

    // JC_TRACE << "A: " << m_cpu.a << " B: " << m_cpu.b << " C: " << m_cpu.c
    //     << '\n';
    // for (size_t i = 0; i < m_program_size; i++) {
    //     JC_TRACE << (int)m_program[i] << '\n';
    // }
}

void Day17::execute() {
    switch (get_instruction()) {
        case Instruction::adv: {
            const int64_t c = combo();
            m_cpu.a /= c == 0 ? 1 : (2 << (c - 1));
            break;
        }
        case Instruction::bxl: {
            m_cpu.b ^= literal();
            break;
        }
        case Instruction::bst: {
            m_cpu.b = combo() & 0b0111;
            break;
        }
        case Instruction::jnz: {
            if (m_cpu.a != 0) {
                m_cpu.ip = literal() - 2;
            }
            break;
        }
        case Instruction::bxc: {
            m_cpu.b ^= m_cpu.c;
            break;
        }
        case Instruction::out: {
            m_output[m_output_size++] = (combo() & 0b0111) + '0';
            // JC_INFO << "Output: " << m_output[m_output_size - 1] << '\n';
            break;
        }
        case Instruction::bdv: {
            const int64_t c = combo();
            m_cpu.b = m_cpu.a / (c == 0 ? 1 : (2 << (c - 1)));
            break;
        }
        case Instruction::cdv: {
            const int64_t c = combo();
            m_cpu.c = m_cpu.a / (c == 0 ? 1 : (2 << (c - 1)));
            break;
        }
    }
    m_cpu.ip += 2;
}

bool Day17::next_digit(const int64_t a, const size_t pos) {
    // JC_TRACE_FN_ARGS(a, pos);
    // JC_ASSERT(pos < 100 , "Invalid position");

    for (int i = 0; i < 8; i++) {
        m_cpu.a = a + i;
        m_cpu.b = 0;
        m_cpu.c = 0;
        m_cpu.ip = 0;
        m_output_size = 0;

        while (static_cast<size_t>(m_cpu.ip) < m_program_size) {
            execute();
        }

        bool valid = true;
        for (size_t j = 0; j < m_program_size - pos; j++) {
            if (m_output[m_output_size - j - 1] != m_program[m_program_size - j - 1] + '0') {
                valid = false;
                break;
            }
        }
        if (valid && pos == 0) {
            m_result.part2 = std::to_string(a + i);
            return true;
        }
        if (valid && next_digit((a + i) * 8, pos - 1)) {
            return true;
        }
    }

    return false;
}

void Day17::calculate(const std::string &file_content) {
    parse_input(file_content);

    while (static_cast<size_t>(m_cpu.ip) < m_program_size) {
        // JC_TRACE << "A: " << m_cpu.a << " B: " << m_cpu.b << " C: " << m_cpu.c
        //     << " Ip: " << static_cast<int32_t>(m_cpu.ip)
        //     << " Instruction: " << static_cast<int32_t>(get_instruction())
        //     << " Combo: " << combo() << " Literal: " << literal() << '\n';
        execute();
    }

    std::stringstream s;
    s << m_output[0];
    for (size_t i = 1; i < m_output_size; i++) {
        s << ',' << m_output[i];
    }
    m_result.part1 = s.str();

    next_digit(0, m_program_size - 1);
}

}  // namespace AOC
