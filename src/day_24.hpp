#pragma once

#include "day.hpp"
namespace AOC {

class Day24 final : public StringLineDay {
   public:
    Day24() : StringLineDay(24) {}

   protected:
    void process_line(const std::string &line) override;
    StringResult string_expected() override { return {"53755311654662", "dkr,ggk,hhh,htp,rhv,z05,z15,z20"}; }
    void post_processing() override;

   private:
    uint64_t m_x = 0, m_y = 0;
    bool m_parsing_xy = true;

    void parse_xy(const std::string &line);
    void parse_gate(const std::string &line);

    enum class GateType {
        AND,
        OR,
        XOR,
        INPUT,
    };

    class Gate {
       public:
        bool get();
        bool eq(const char *id);

        void init(const char *id);

        void init_and(Gate *left, Gate *right);
        void init_or(Gate *left, Gate *right);
        void init_xor(Gate *left, Gate *right);
        void init_input(uint64_t *in_vec, uint32_t bit);

        void print();

        bool is_sus(Gate *all_gates, size_t gate_count);
        bool lt(const Gate &other) const {
            return m_id[0] != other.m_id[0]   ? m_id[0] < other.m_id[0]
                   : m_id[1] != other.m_id[1] ? m_id[1] < other.m_id[1]
                                              : m_id[2] < other.m_id[2];
        }

        void print(std::ostream &os) const {
            os << m_id[0] << m_id[1] << m_id[2];
        }
       private:
        char m_id[3]{};
        GateType type{};
        Gate *left{}, *right{};
        uint64_t *in_vec{};
        uint32_t bit{};
    };

    Gate m_gates[512];
    size_t m_gate_count = 0;

    Gate *find_gate(const char *id);

    Gate *m_output[64];
    size_t m_output_max = 0;
};

}  // namespace AOC
