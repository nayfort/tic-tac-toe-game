#pragma once
#include <algorithm>
#include <cstdint>
#include <istream>
#include <limits>
#include <ostream>
#include <sstream>
#include <string>

namespace CrossGame {
// Ignore malformed records without preventing the game from starting.
inline std::uint64_t readScores(std::istream& input, std::ostream& output) {
    std::uint64_t next = 1;
    std::string line;
    while (std::getline(input, line)) {
        std::istringstream record(line);
        std::string id, date, first, second, extra;
        if (!std::getline(record, id, ';') || !std::getline(record, date, ';') ||
            !std::getline(record, first, ';') || !std::getline(record, second, ';') ||
            std::getline(record, extra, ';') || id.empty() || date.empty() ||
            first.find('=') == std::string::npos || second.find('=') == std::string::npos ||
            id.find_first_not_of("0123456789") != std::string::npos) continue;
        try {
            auto number = std::stoull(id);
            if (number == 0 || number >= std::numeric_limits<std::uint64_t>::max()) continue;
            next = std::max(next, static_cast<std::uint64_t>(number + 1));
            output << number << '\t' << date << '\t' << first << '\t' << second << '\n';
        } catch (const std::exception&) { /* Invalid or overflowing identifier. */ }
    }
    return next;
}
}
