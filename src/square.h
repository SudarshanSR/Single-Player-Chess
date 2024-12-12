#ifndef SQUARE_H
#define SQUARE_H

#include <compare>
#include <cstdint>
#include <format>
#include <string>

using Rank = std::int32_t;
using File = std::int32_t;

struct Square final {
    Rank rank;
    File file;

    [[nodiscard]] explicit operator std::string() const {
        return std::format(
            "{}{}", static_cast<char>('a' + this->file), 8 - this->rank
        );
    }

    [[nodiscard]] bool operator==(Square const &) const = default;

    [[nodiscard]] auto operator<=>(Square const &) const = default;
};

#endif // SQUARE_H