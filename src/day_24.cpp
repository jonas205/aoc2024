#include "day_24.hpp"
namespace AOC {

void Day24::parse_xy(const std::string &line) {
    bool is_x = line[0] == 'x';
    size_t bit_pos = 10 * (line[1] - '0') + line[2] - '0';
    uint64_t bit = static_cast<uint64_t>(line[5] - '0') << bit_pos;

    m_gates[m_gate_count].init(line.c_str());
    if (is_x) {
        m_x |= bit;
        m_gates[m_gate_count++].init_input(&m_x, bit_pos);
    } else {
        m_y |= bit;
        m_gates[m_gate_count++].init_input(&m_y, bit_pos);
    }
}

void Day24::parse_gate(const std::string &line) {
    char id_left[3], id_right[3], id_out[3];

    id_left[0] = line[0];
    id_left[1] = line[1];
    id_left[2] = line[2];

    GateType type = (line[4] == 'X')   ? GateType::XOR
                    : (line[4] == 'A') ? GateType::AND
                                       : GateType::OR;
    size_t i = (type == GateType::OR) ? 7 : 8;

    id_right[0] = line[i++];
    id_right[1] = line[i++];
    id_right[2] = line[i++];

    i += 4;

    id_out[0] = line[i++];
    id_out[1] = line[i++];
    id_out[2] = line[i];

    // trace("Left: " << id_left[0] << id_left[1] << id_left[2]
    //                << " Right: " << id_right[0] << id_right[1] << id_right[2]
    //                << " Out: " << id_out[0] << id_out[1] << id_out[2]);

    Gate *left = find_gate(id_left);
    Gate *right = find_gate(id_right);
    Gate *out = find_gate(id_out);

    switch (GateType(type)) {
        case GateType::AND:
            out->init_and(left, right);
            break;
        case GateType::OR:
            out->init_or(left, right);
            break;
        case GateType::XOR:
            out->init_xor(left, right);
            break;
        default:
            die("Invalid gate type");
    }

    if (id_out[0] != 'z') {
        return;
    }

    size_t id = 10 * (id_out[1] - '0') + id_out[2] - '0';
    m_output[id] = out;
    if (id > m_output_max) {
        m_output_max = id;
    }
}

Day24::Gate *Day24::find_gate(const char *id) {
    for (size_t i = 0; i < m_gate_count; i++) {
        if (m_gates[i].eq(id)) {
            return &m_gates[i];
        }
    }
    m_gates[m_gate_count].init(id);
    return &m_gates[m_gate_count++];
}

void Day24::process_line(const std::string &line) {
    if (line[0] == '\n') {
        m_parsing_xy = false;
        return;
    } else if (m_parsing_xy) {
        parse_xy(line);
    } else {
        parse_gate(line);
    }
}

void Day24::post_processing() {
    // trace("X: " << m_x << " Y: " << m_y);

    uint64_t z = 0;
    for (size_t i = 0; i < m_output_max + 1; i++) {
        z |= static_cast<uint64_t>(m_output[i]->get()) << i;
    }
    m_result.part1 = std::to_string(z);

    Gate *sus_gates[16];
    size_t sus_count = 0;

    for (size_t i = 0; i < m_gate_count; i++) {
        if (m_gates[i].is_sus(m_gates, m_gate_count)) {
            JC_ASSERT(sus_count < 16, "Too many sus gates");
            sus_gates[sus_count++] = &m_gates[i];
        }
    }

    std::sort(sus_gates, sus_gates + sus_count,
              [](Gate *a, Gate *b) { return a->lt(*b); }
              );

    std::stringstream s;
    sus_gates[0]->print(s);
    for (size_t i = 1; i < sus_count; i++) {
        s << ',';
        sus_gates[i]->print(s);
    }
    m_result.part2 = s.str();

}

void Day24::Gate::print() {
    trace("Gate: " << m_id[0] << m_id[1] << m_id[2]);
}

bool Day24::Gate::is_sus(Gate *all_gates, size_t gate_count) {
    if (type == GateType::INPUT) {
        return false;
    }

    bool is_output = m_id[0] == 'z';
    bool is_input = left->type == GateType::INPUT || right->type == GateType::INPUT;

    if (is_output && m_id[1] == '0' && m_id[2] == '0') {
        return false;
    }
    if (is_input && left->eq("y00") && right->eq("x00")) {
        return false;
    }

    if (is_output && type != GateType::XOR) {
        return m_id[1] != '4' || m_id[2] != '5';
    }
    if (!is_input && !is_output && type == GateType::XOR) {
        return true;
    }

    if (type == GateType::AND) {
        bool found_other = false;
        for (size_t i = 0; i < gate_count; i++) {
            Gate *g = &all_gates[i];
            if (g->type == GateType::INPUT) {
                continue;
            }
            if (g->left->eq(m_id) && GateType::OR == g->type) {
                found_other = true;
                break;
            }
            if (g->right->eq(m_id) && GateType::OR == g->type) {
                found_other = true;
                break;
            }
        }
        if (!found_other) {
            return true;
        }
    }

    if (is_input && type == GateType::XOR) {
        bool found_other = false;
        for (size_t i = 0; i < gate_count; i++) {
            Gate *g = &all_gates[i];
            if (g->type == GateType::INPUT) {
                continue;
            }
            if (g->left->eq(m_id) && GateType::XOR == g->type) {
                found_other = true;
                break;
            }
            if (g->right->eq(m_id) && GateType::XOR == g->type) {
                found_other = true;
                break;
            }
        }
        if (!found_other) {
            return true;
        }
    }
    return false;
}

bool Day24::Gate::get() {
    switch (type) {
        case GateType::AND:
            return left->get() && right->get();
        case GateType::OR:
            return left->get() || right->get();
        case GateType::XOR:
            return left->get() ^ right->get();
        case GateType::INPUT:
            return (*in_vec & (1LL << bit)) > 0;
        default:
            die("Invalid gate type");
    }
}

bool Day24::Gate::eq(const char *id) {
    return id[0] == m_id[0] && id[1] == m_id[1] && id[2] == m_id[2];
}
void Day24::Gate::init(const char *id) {
    m_id[0] = id[0];
    m_id[1] = id[1];
    m_id[2] = id[2];

    // if (m_id[0] == 'x' || m_id[0] == 'y' || m_id[0] == 'z') {
    //     return;
    // }
    // JC_COUT << "signal " << id[0] << id[1] << id[2] << " : bit := '0';\n";
    // if (m_id[0] == 'x') {
    //     JC_COUT << m_id[0] << m_id[1] << m_id[2] << " <= x(" << m_id[1] << m_id[2] << ");\n";
    // }
    // if (m_id[0] == 'y') {
    //     JC_COUT << m_id[0] << m_id[1] << m_id[2] << " <= y(" << m_id[1] << m_id[2] << ");\n";
    // }
    // if (m_id[0] == 'z') {
    //     JC_COUT << "z(" << m_id[1] << m_id[2] << ") <= " << m_id[0] << m_id[1] << m_id[2] << ";\n";
    // }
}
void Day24::Gate::init_and(Gate *l, Gate *r) {
    type = GateType::AND;
    left = l;
    right = r;
    // JC_COUT << m_id[0] << m_id[1] << m_id[2] << " <= " << l->m_id[0]
    //         << l->m_id[1] << l->m_id[2] << " and " << r->m_id[0] << r->m_id[1]
    //         << r->m_id[2] << ";\n";
}
void Day24::Gate::init_or(Gate *l, Gate *r) {
    type = GateType::OR;
    left = l;
    right = r;
    // JC_COUT << m_id[0] << m_id[1] << m_id[2] << " <= " << l->m_id[0]
    //         << l->m_id[1] << l->m_id[2] << " or " << r->m_id[0] << r->m_id[1]
    //         << r->m_id[2] << ";\n";
}
void Day24::Gate::init_xor(Gate *l, Gate *r) {
    type = GateType::XOR;
    left = l;
    right = r;
    // JC_COUT << m_id[0] << m_id[1] << m_id[2] << " <= " << l->m_id[0]
    //         << l->m_id[1] << l->m_id[2] << " xor " << r->m_id[0] << r->m_id[1]
    //         << r->m_id[2] << ";\n";
}
void Day24::Gate::init_input(uint64_t *in, uint32_t b) {
    type = GateType::INPUT;
    in_vec = in;
    bit = b;
}

}  // namespace AOC
