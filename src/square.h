#ifndef SQUARE_H
#define SQUARE_H

#include <compare>
#include <cstdint>

using Rank = std::int32_t;
using File = std::int32_t;

struct Square final {
    Rank rank;
    File file;

    [[nodiscard]] bool operator==(Square const &) const = default;

    [[nodiscard]] auto operator<=>(Square const &) const = default;
};

#endif // SQUARE_H