#ifndef SQUARE_H
#define SQUARE_H

#include <compare>
#include <cstdint>

using Rank = std::int16_t;
using File = std::int16_t;

struct Square final {
    Rank rank;
    File file;

    [[nodiscard]] bool operator==(Square const &) const = default;

    [[nodiscard]] auto operator<=>(Square const &) const = default;
};

#endif // SQUARE_H