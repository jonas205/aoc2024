#pragma once

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <cstdio>
#include <string>

#include "jcpp_instrumentor.hpp"
#include "jcpp_log.hpp"

namespace AOC {

typedef uint64_t ResultType;

struct Result {
    ResultType part1 = 0;
    ResultType part2 = 0;
};

struct StringResult {
    std::string part1;
    std::string part2;
};

class Day {
   public:
    explicit Day(const int n) : m_day(n) {}
    virtual ~Day() = default;

    virtual void run() {
        std::stringstream s;
        s << " Day " << m_day << ':';
        const std::string day_string = s.str();

        JC_PROFILE_SCOPE(day_string.c_str());
        info(day_string);
        read_file();
        check_result();
    }

   protected:
    virtual void calculate(const std::string &file_content) = 0;
    virtual Result expected() = 0;

    Result m_result;
    int m_day;

    virtual void check_result() {
        const auto [part1, part2] = expected();

        if (m_result.part1 == part1) {
            info("   Part 1: " << m_result.part1);
        } else {
            trace("(Part 2: " << m_result.part2 << ')');
            die("Part 1: " << m_result.part1 << " (expected " << part1 << ')')
        }

        if (m_result.part2 == part2) {
            info("   Part 2: " << m_result.part2);
        } else {
            die("Part 2: " << m_result.part2 << " (expected " << part2 << ')');
        }
    }

   private:
    void read_file() {
        struct stat sb {};

        const std::string f_name = "input/day_" + std::to_string(m_day);
        const int fd = open(f_name.c_str(), O_RDONLY);
        JC_ASSERT(fd != -1, "Failed to open file: " << f_name);
        fstat(fd, &sb);

        const auto data = static_cast<char *>(
            mmap(nullptr, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0));

        JC_PROFILE_SCOPE("calculate");
        calculate(std::string(data, sb.st_size));
    }
};

class StringDay : public Day {
   public:
    explicit StringDay(const int n) : Day(n) {}

   protected:
    virtual void check_result() override {
        const auto [part1, part2] = string_expected();

        if (m_result.part1 == part1) {
            info("   Part 1: " << m_result.part1);
        } else {
            trace("(Part 2: " << m_result.part2 << ')');
            die("Part 1: " << m_result.part1 << " (expected " << part1 << ')');
        }
        if (m_result.part2 == part2) {
            info("   Part 2: " << m_result.part2);
        } else {
            die("Part 2: " << m_result.part2 << " (expected " << part2 << ')');
        }
    }

    virtual Result expected() override { return {0, 0}; }
    virtual StringResult string_expected() = 0;

    StringResult m_result;
};

class LineByLineDay : public Day {
   public:
    explicit LineByLineDay(int n) : Day(n) {}
    ~LineByLineDay() override = default;

   protected:
    virtual void process_line(const std::string &line) = 0;
    virtual void post_processing() {}

    void run() override {
        std::stringstream s;
        s << "Day " << m_day << ':';
        const std::string day_string = s.str();

        JC_PROFILE_SCOPE(day_string.c_str());
        info(day_string);

        struct stat sb {};

        const std::string f_name = "input/day_" + std::to_string(m_day);
        const int fd = open(f_name.c_str(), O_RDONLY);
        JC_ASSERT(fd != -1, "Failed to open file: " << f_name);
        fstat(fd, &sb);

        {
            auto data = static_cast<char *>(
                mmap(nullptr, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0));
            long cntr = 0;

            JC_PROFILE_SCOPE("process_line");
            while (cntr < sb.st_size) {
                int line_len = 0;
                const char *line = data;

                while (*data != '\n' && cntr < sb.st_size) {
                    line_len++;
                    data++;
                    cntr++;
                }
                line_len++;
                data++;
                cntr++;
                process_line(std::string(line, line_len));
            }
        }
        {
            JC_PROFILE_SCOPE("post_processing");
            post_processing();
        }
        check_result();
    }

    void calculate(const std::string &file_content) override {
        (void)file_content;
        die("Unreachable code");
    }
    Result expected() override = 0;
};

class StringLineDay : public LineByLineDay {
   public:
    explicit StringLineDay(const int n) : LineByLineDay(n) {}

   protected:
    virtual void check_result() override {
        const auto [part1, part2] = string_expected();

        if (m_result.part1 == part1) {
            info("   Part 1: " << m_result.part1);
        } else {
            trace("(Part 2: " << m_result.part2 << ')');
            die("Part 1: " << m_result.part1 << " (expected " << part1 << ')');
        }
        if (m_result.part2 == part2) {
            info("   Part 2: " << m_result.part2);
        } else {
            die("Part 2: " << m_result.part2 << " (expected " << part2 << ')');
        }
    }

    virtual Result expected() override { return {0, 0}; }
    virtual StringResult string_expected() = 0;

    StringResult m_result;
};

}  // namespace AOC
