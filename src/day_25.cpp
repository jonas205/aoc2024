
#include "day_25.hpp"
#include <cstring>
namespace AOC {

void Day25::process_line(const std::string &line) {
    // trace_prefix << line;

    if (m_line_count == 7) {
        m_line_count = 0;
        std::memset(m_parsing, 0, sizeof(m_parsing));
        return;
    }

    for (size_t i = 0; i < 5; i++) {
        m_parsing[i] += line[i] == '#' ? 1 : 0;
    }

    m_line_count++;
    if (m_line_count != 7) {
        return;
    }

    if (line[0] == '#') {
        JC_ASSERT(m_key_count < MAX_SIZE, "Too many keys");
        m_keys[m_key_count++].init(m_parsing);
        // info("FOUND KEY");
        // m_keys[m_key_count - 1].print();
    } else {
        JC_ASSERT(m_lock_count < MAX_SIZE, "Too many locks");
        m_locks[m_lock_count++].init(m_parsing);
        // info("FOUND LOCK");
        // m_locks[m_lock_count - 1].print();
    }
}
void Day25::post_processing() {
    for (size_t i = 0; i < m_lock_count; i++) {
        for (size_t j = 0; j < m_key_count; j++) {
            if (match(m_keys[j], m_locks[i])) {
                m_result.part1++;
                // info("Match found");
                // m_keys[j].print();
                // m_locks[i].print();
            }
        }
    }
}

void Day25::Common::init(uint8_t *v) {
    for (size_t i = 0; i < 5; i++) {
        value[i] = v[i] - 1;
    }
}

bool Day25::match(const Key &key, const Lock &lock) {
    for (size_t i = 0; i < 5; i++) {
        if (key.value[i] + lock.value[i] >= 6) {
            return false;
        }
    }
    return true;
}

void Day25::Common::print() {
    trace(static_cast<uint32_t>(value[0])
          << ',' << static_cast<uint32_t>(value[1]) << ','
          << static_cast<uint32_t>(value[2]) << ','
          << static_cast<uint32_t>(value[3]) << ','
          << static_cast<uint32_t>(value[4]));
}
}  // namespace AOC
