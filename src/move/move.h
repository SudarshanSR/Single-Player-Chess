#ifndef MOVE_H
#define MOVE_H

#include "../square.h"

struct Move final {
    Square start;
    Square end;

    [[nodiscard]] auto is_valid() const -> bool;

    [[nodiscard]] bool operator==(Move const &) const = default;

    [[nodiscard]] auto operator<=>(Move const &) const = default;
};

#endif // MOVE_H
