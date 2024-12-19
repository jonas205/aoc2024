#include "day_9.hpp"
#include <cstring>

namespace AOC {

// static void print_disk(int32_t *disk, size_t disk_end) {
//     for (size_t i = 0; i <= disk_end; i++) {
//         if (disk[i] == -1) {
//             std::cout << '.';
//         } else {
//             std::cout << disk[i];
//         }
//     }
//     std::cout << '\n';
// };

static uint64_t part_1(int32_t *disk, size_t disk_end) {
    uint64_t sum = 0;
    size_t i = 0;
    while (true) {
        // Find next gap
        for (; disk[i] != -1; i++)
            ;
        // Find last number
        for (; disk[disk_end] == -1; disk_end--)
            ;

        if (i > disk_end) {
            break;
        }

        disk[i++] = disk[disk_end--];
    }

    for (size_t j = 0; j <= disk_end; j++) {
        sum += j * disk[j];
    }
    return sum;
}

static uint64_t part_2(int32_t *disk, const size_t disk_end) {
    uint64_t sum = 0;

    size_t end = disk_end;
    // print_disk(disk, disk_end);

    int32_t last_id = INT32_MAX;

    size_t starts[10] = {0};

    while (end > 0) {
        for (; disk[end] == -1; end--)
            ;

        const int32_t id = disk[end];
        size_t size = 1;

        for (; disk[end - size] == id; size++)
            ;

        if (id > last_id) {
            end -= size;
            continue;
        }
        if (id == 0) {
            break;
        }

        last_id = id;

        // JC_TRACE << "ID: " << id << " Size: " << size << '\n';

        // JC_ASSERT(size < 10, "Size is too big");
        size_t &start = starts[size];

        bool found = false;
        while (start <= end) {
            for (; disk[start] != -1; start++)
                ;

            if (start > end) {
                break;
            }
            size_t gap_size = 1;
            for (; disk[start + gap_size] == -1; gap_size++)
                ;

            if (gap_size >= size) {
                found = true;
                break;
            }
            start += gap_size;
        }

        if (found) {
            for (size_t i = 0; i < size; i++) {
                disk[start + i] = id;
                disk[end - i] = -1;
            }
            // print_disk(disk, disk_end);
        }

        end -= size;
    }

    for (size_t i = 0; i <= disk_end; i++) {
        if (disk[i] != -1) {
            sum += i * disk[i];
        }
    }
    return sum;
}

void Day9::calculate(const std::string &content) {
    int32_t *disk;
    int32_t *disk2;
    size_t disk_end;
    {
        JC_PROFILE_SCOPE("Allocating");
        disk = new int32_t[content.length() * 10];
        disk2 = new int32_t[content.length() * 10];
    }
    {
        JC_PROFILE_SCOPE("Parsing Input");
        disk_end = 0;
        for (size_t i = 0; i < content.length(); i += 2) {
            const size_t id = i / 2;
            for (int j = 0; j < content[i] - '0'; j++) {
                disk[disk_end++] = static_cast<int32_t>(id);
            }
            for (int j = 0; j < content[i + 1] - '0'; j++) {
                disk[disk_end++] = -1;
            }
        }
        disk_end--;
    }
    {
        JC_PROFILE_SCOPE("Memcpy");
        memcpy(disk2, disk, (disk_end + 1) * sizeof(int32_t));
    }

    {
        JC_PROFILE_SCOPE("Part 1");
        m_result.part1 = part_1(disk, disk_end);
    }
    {
        JC_PROFILE_SCOPE("Part 2");
        m_result.part2 = part_2(disk2, disk_end);
    }
    {
        JC_PROFILE_SCOPE("Freeing");
        delete[] disk;
        delete[] disk2;
    }
}

}  // namespace AOC
