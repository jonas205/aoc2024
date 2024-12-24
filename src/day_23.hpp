
#pragma once

#include <sstream>
#include "day.hpp"
namespace AOC {

class Day23 final : public StringLineDay {
   public:
    Day23() : StringLineDay(23) {}

   protected:
    void process_line(const std::string &line) override;
    StringResult string_expected() override { return {"1173", "cm,de,ez,gv,hg,iy,or,pw,qu,rs,sn,uc,wq"}; }
    void post_processing() override;

   private:
    typedef size_t ID;
    static ID calc_id(const char a, const char b) {
        return (a - 'a') * 26 + (b - 'a');
    }

    static void calc_from_id(ID id, char &a, char &b) {
        a = 'a' + (id / 26);
        b = 'a' + (id % 26);
    }

    bool start_with_t(ID id) {
        static constexpr ID t = ('t' - 'a');
        return (id / 26) == t;
    }

    class Node {
        static constexpr size_t MAX_NEIGHBORS = 32;

       public:
        bool initialized() const { return m_id != 1024; }
        void initialize(ID id) { m_id = id; }

        void add_neighbor(Node *id) {
            JC_ASSERT(m_neighbor_count < MAX_NEIGHBORS, "Too many neighbors");
            m_neighbor[m_neighbor_count++] = id;
        }
        Node *neighbor(size_t i) const { return m_neighbor[i]; }
        size_t neighbor_count() const { return m_neighbor_count; }
        ID id() const { return m_id; }

        bool contains(ID id) const {
            for (size_t i = 0; i < m_neighbor_count; i++) {
                if (m_neighbor[i]->id() == id) {
                    return true;
                }
            }
            return false;
        }

       private:
        ID m_id = 1024;
        Node *m_neighbor[MAX_NEIGHBORS]{};
        size_t m_neighbor_count = 0;
    };

    class NodeSet {
        static constexpr size_t MAX_NODES = 32;

       public:
        bool try_add(Node *n) {
            JC_ASSERT(node_count < MAX_NODES, "Too many nodes");
            for (size_t i = 0; i < node_count; i++) {
                if (!nodes[i]->contains(n->id())) {
                    return false;
                }
            }
            nodes[node_count++] = n;
            return true;
        }
        bool contains(const Node *n) {
            for (size_t i = 0; i < node_count; i++) {
                if (nodes[i] == n) {
                    return true;
                }
            }
            return false;
        }
        void force_add(Node *n) {
            JC_ASSERT(node_count < MAX_NODES, "Too many nodes");
            nodes[node_count++] = n;
        }
        size_t size() const { return node_count; }
        void assing_sol_string(std::string &str) {
            std::sort(
                nodes, nodes + node_count,
                [](const Node *a, const Node *b) { return a->id() < b->id(); });
            std::stringstream s;
            char a, b;
            calc_from_id(nodes[0]->id(), a, b);
            s << a << b;
            for (size_t i = 1; i < node_count; i++) {
                calc_from_id(nodes[i]->id(), a, b);
                s << ',' << a << b;
            }
            str = s.str();
        }

       private:
        Node *nodes[MAX_NODES]{};
        size_t node_count = 0;
    };

    static constexpr size_t MAX_NODE_SETS = 2 * 1024;
    NodeSet m_node_set[MAX_NODE_SETS]{};
    size_t m_node_set_count = 0;

    Node m_nodes[1024]{};
    size_t m_node_count = 0;
};
}  // namespace AOC
