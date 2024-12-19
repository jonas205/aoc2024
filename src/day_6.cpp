#include "day_6.hpp"

namespace AOC {

bool Day6Room::obstructed(const size_t x, const size_t y) const {
    if (x >= width || y >= height) {
        return false;
    }
    return tiles[x][y] == TILE_OBSTRUCTED;
}

void Day6Room::visited(const size_t x, const size_t y,
                       const PlayerState state) {
    if (x >= width || y >= height) {
        return;
    }

    switch (state) {
        case PlayerState::LEFT:
            tiles[x][y] |= TILE_VISITED_LEFT;
            break;
        case PlayerState::UP:
            tiles[x][y] |= TILE_VISITED_UP;
            break;
        case PlayerState::RIGHT:
            tiles[x][y] |= TILE_VISITED_RIGHT;
            break;
        case PlayerState::DOWN:
            tiles[x][y] |= TILE_VISITED_DOWN;
            break;
    }
}

bool Day6Room::visited(const size_t x, const size_t y) const {
    if (x >= width || y >= height) {
        return true;
    }
    return tiles[x][y] > TILE_OBSTRUCTED;
}

void Day6Room::player_start(const size_t x, const size_t y,
                            const PlayerState dir) {
    tile(x, y, Day6Room::TILE_VISITED_UP);
    player.x = x;
    player.y = y;
    player.state = dir;
}

void Day6::process_line(const std::string &line) {
    if (m_room.width == 0) {
        m_room.width = line.size() - 1;
    }
    for (size_t i = 0; i < line.size(); i++) {
        switch (line[i]) {
            case '.':
                m_room.tile(i, m_room.height, Day6Room::TILE_FREE);
                break;
            case '#':
                m_room.tile(i, m_room.height, Day6Room::TILE_OBSTRUCTED);
                break;
            case '^':
                m_room.player_start(i, m_room.height,
                                    Day6Room::PlayerState::UP);
                break;
            case '>':
                m_room.player_start(i, m_room.height,
                                    Day6Room::PlayerState::RIGHT);
                break;
            case '<':
                m_room.player_start(i, m_room.height,
                                    Day6Room::PlayerState::LEFT);
                break;
            case 'v':
                m_room.player_start(i, m_room.height,
                                    Day6Room::PlayerState::DOWN);
                break;
            case '\n':
                break;
            default:
                die("Invalid character in input: "
                    << line[i] << " (0x" << std::hex
                    << static_cast<int>(line[i]) << ')');
        }
    }

    m_room.height++;
}

uint32_t Day6Room::count_tiles() {
    uint32_t erg = 1;
    while (true) {
        size_t dx = 0;
        size_t dy = 0;

        switch (player.state) {
            case PlayerState::UP:
                dy--;
                break;
            case PlayerState::RIGHT:
                dx++;
                break;
            case PlayerState::DOWN:
                dy++;
                break;
            case PlayerState::LEFT:
                dx--;
                break;
        }

        size_t x = player.x + dx;
        size_t y = player.y + dy;

        while (!obstructed(x, y)) {
            if (x >= width || y >= height) {
                return erg;
            }

            if (tiles[x][y] == TILE_FREE) {
                erg++;
            }
            visited(x, y, player.state);

            x += dx;
            y += dy;
        }

        player.x = x - dx;
        player.y = y - dy;

        switch (player.state) {
            case PlayerState::UP:
                player.state = PlayerState::RIGHT;
                break;
            case PlayerState::RIGHT:
                player.state = PlayerState::DOWN;
                break;
            case PlayerState::DOWN:
                player.state = PlayerState::LEFT;
                break;
            case PlayerState::LEFT:
                player.state = PlayerState::UP;
                break;
        }
    }
}

bool Day6Room::is_loop() {
    while (true) {
        size_t dx = 0;
        size_t dy = 0;

        switch (player.state) {
            case PlayerState::UP:
                dy--;
                break;
            case PlayerState::RIGHT:
                dx++;
                break;
            case PlayerState::DOWN:
                dy++;
                break;
            case PlayerState::LEFT:
                dx--;
                break;
        }

        size_t x = player.x + dx;
        size_t y = player.y + dy;

        while (!obstructed(x, y)) {
            if (x >= width || y >= height) {
                return false;
            }

            x += dx;
            y += dy;
        }

        x -= dx;
        y -= dy;

        uint8_t was_visited = tiles[x][y];
        visited(x, y, player.state);

        player.x = x;
        player.y = y;

        switch (player.state) {
            case PlayerState::UP:
                if (TILE_VISITED_UP & was_visited) {
                    return true;
                }
                break;
            case PlayerState::RIGHT:
                if (TILE_VISITED_RIGHT & was_visited) {
                    return true;
                }
                break;
            case PlayerState::DOWN:
                if (TILE_VISITED_DOWN & was_visited) {
                    return true;
                }
                break;
            case PlayerState::LEFT:
                if (TILE_VISITED_LEFT & was_visited) {
                    return true;
                }
                break;
        }

        switch (player.state) {
            case PlayerState::UP:
                player.state = PlayerState::RIGHT;
                break;
            case PlayerState::RIGHT:
                player.state = PlayerState::DOWN;
                break;
            case PlayerState::DOWN:
                player.state = PlayerState::LEFT;
                break;
            case PlayerState::LEFT:
                player.state = PlayerState::UP;
                break;
        }
    }
}

void Day6Room::print() const {
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            switch (tiles[x][y]) {
                case TILE_FREE:
                    std::cout << '.';
                    break;
                case TILE_OBSTRUCTED:
                    std::cout << '#';
                    break;
                default:
                    std::cout << 'X';
                    break;
            }
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

void Day6::post_processing() {
    Day6Room original = m_room;
    {
        JC_PROFILE_SCOPE("Part 1");
        m_result.part1 = m_room.count_tiles();
    }

    original.tiles[original.player.x][original.player.y] = Day6Room::TILE_FREE;

    JC_PROFILE_SCOPE("Part 2");

    size_t sum = 0;

#pragma omp parallel for collapse(2) reduction(+ : sum) schedule(dynamic)
    for (size_t y = 0; y < m_room.height; y++) {
        for (size_t x = 0; x < m_room.width; x++) {
            if (x == original.player.x && y == original.player.y) {
                continue;
            }
            if (!m_room.visited(x, y)) {
                continue;
            }
            Day6Room room = original;
            room.tiles[x][y] = Day6Room::TILE_OBSTRUCTED;

            if (room.is_loop()) {
                sum++;
            }
        }
    }

#pragma omp atomic
    m_result.part2 += sum;
}
}  // namespace AOC
