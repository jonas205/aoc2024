#include "day_23.hpp"
#include "day.hpp"

namespace AOC {
void Day23::process_line(const std::string &line) {
    char a = line[0], b = line[1], c = line[3], d = line[4];

    ID id_0 = calc_id(a, b);
    ID id_1 = calc_id(c, d);

    if (!m_nodes[id_0].initialized()) {
        m_nodes[id_0].initialize(id_0);
    }

    if (!m_nodes[id_1].initialized()) {
        m_nodes[id_1].initialize(id_1);
    }

    m_nodes[id_0].add_neighbor(&m_nodes[id_1]);
    m_nodes[id_1].add_neighbor(&m_nodes[id_0]);
}

void Day23::post_processing() {
    int32_t sol_1 = 0;
    {
        JC_PROFILE_SCOPE("Part 1");
        for (char c = 'a'; c <= 'z'; c++) {
            ID id = calc_id('t', c);
            if (!m_nodes[id].initialized()) {
                continue;
            }
            // trace(c);
            for (size_t i = 0; i < m_nodes[id].neighbor_count(); i++) {
                Node *n = m_nodes[id].neighbor(i);
                if (n->id() > id && start_with_t(n->id())) {
                    continue;
                }
                // Part1
                for (size_t j = 0; j < n->neighbor_count(); j++) {
                    const Node *nn = n->neighbor(j);
                    if (nn->id() > n->id()) {
                        continue;
                    }
                    if (start_with_t(nn->id()) && nn->id() > id) {
                        continue;
                    }
                    if (nn->contains(id)) {
                        // char a, b, c, d, e, f;
                        // calc_from_id(id, a, b);
                        // calc_from_id(n->id(), c, d);
                        // calc_from_id(nn->id(), e, f);
                        // trace("Found " << c << " (" << a << b << ") - (" << c << d
                        //                << ") - (" << e << f << ")");
                        sol_1++;
                    }
                }
            }
        }
    }

    {
        JC_PROFILE_SCOPE("Part 2");

        {
            JC_PROFILE_SCOPE("Generate Sets");
            for (char a = 'a'; a <= 'z'; a++) {
                for (char b = 'a'; b <= 'z'; b++) {
                    ID id = calc_id(a, b);
                    Node *n = &m_nodes[id];
                    if (!n->initialized()) {
                        continue;
                    }
                    for (size_t i = 0; i < m_nodes[id].neighbor_count(); i++) {
                        Node *nn = m_nodes[id].neighbor(i);
                        // char c, d;
                        // calc_from_id(n->id(), c, d);
                        // trace("Checking (" << a << b << ") - (" << c << d
                        //                   << ")");
                        // Part2
                        bool found = false;
                        for (size_t j = 0; j < m_node_set_count; j++) {
                            if (m_node_set[j].contains(n) &&
                                m_node_set[j].try_add(nn)) {
                                // trace("Added " << n->id() << " to " << j);
                                found = true;
                            }
                        }
                        if (!found) {
                            // trace("New set");
                            JC_ASSERT(m_node_set_count < MAX_NODE_SETS,
                                      "Too many sets");
                            m_node_set[m_node_set_count].force_add(n);
                            m_node_set[m_node_set_count].force_add(nn);
                            m_node_set_count++;
                        }
                    }
                }
            }
        }

        size_t size_max = 0;
        size_t size_i = 0;

        {
            JC_PROFILE_SCOPE("Find Max Set");

            for (size_t i = 0; i < m_node_set_count; i++) {
                if (m_node_set[i].size() > size_max) {
                    size_max = m_node_set[i].size();
                    size_i = i;
                }
            }
        }

        {
            JC_PROFILE_SCOPE("Generate Part 2");
            m_node_set[size_i].assing_sol_string(m_result.part2);
            m_result.part1 = std::to_string(sol_1);
        }
    }
}

}  // namespace AOC
