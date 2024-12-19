#pragma once

#include <cstring>
#include "day.hpp"
namespace AOC {

struct Day6Room {
    Day6Room() = default;
    Day6Room(const Day6Room &room) {
        width = room.width;
        height = room.height;
        memcpy(tiles, room.tiles, sizeof(tiles));
        player = room.player;
    }

    static constexpr uint8_t TILE_FREE = 0;
    static constexpr uint8_t TILE_OBSTRUCTED = 1;
    static constexpr uint8_t TILE_VISITED_LEFT = 2;
    static constexpr uint8_t TILE_VISITED_RIGHT = 4;
    static constexpr uint8_t TILE_VISITED_UP = 8;
    static constexpr uint8_t TILE_VISITED_DOWN = 16;

    enum class PlayerState { LEFT, UP, RIGHT, DOWN };

    void tile(const size_t x, const size_t y, const uint8_t value) { tiles[x][y] = value; }
    void player_start(size_t x, size_t y, PlayerState dir);

    uint32_t count_tiles();
    bool is_loop();
    void print() const;

    [[nodiscard]] bool obstructed(size_t x, size_t y) const;
    void visited(size_t x, size_t y, PlayerState state);
    [[nodiscard]] bool visited(size_t x, size_t y) const;

    uint8_t tiles[130][130]{};
    size_t width = 0, height = 0;

    struct {
        PlayerState state;
        size_t x, y;
    } player{};
};

class Day6 final : public LineByLineDay {
   public:
    Day6() : LineByLineDay(6) {}

   protected:
    void process_line(const std::string &line) override;
    void post_processing() override;
    Result expected() override { return {4374, 1705}; }

   private:
    Day6Room m_room;
};
}  // namespace AOC
