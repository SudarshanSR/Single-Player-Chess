#ifndef VARS_H
#define VARS_H

#include "colour.h"
#include "square.h"

#include <array>
#include <map>
#include <ranges>

struct Piece;

inline static std::array<std::array<Square, 8>, 8> constexpr k_board{
    Square{0, 0}, Square{0, 1}, Square{0, 2}, Square{0, 3}, Square{0, 4},
    Square{0, 5}, Square{0, 6}, Square{0, 7}, Square{1, 0}, Square{1, 1},
    Square{1, 2}, Square{1, 3}, Square{1, 4}, Square{1, 5}, Square{1, 6},
    Square{1, 7}, Square{2, 0}, Square{2, 1}, Square{2, 2}, Square{2, 3},
    Square{2, 4}, Square{2, 5}, Square{2, 6}, Square{2, 7}, Square{3, 0},
    Square{3, 1}, Square{3, 2}, Square{3, 3}, Square{3, 4}, Square{3, 5},
    Square{3, 6}, Square{3, 7}, Square{4, 0}, Square{4, 1}, Square{4, 2},
    Square{4, 3}, Square{4, 4}, Square{4, 5}, Square{4, 6}, Square{4, 7},
    Square{5, 0}, Square{5, 1}, Square{5, 2}, Square{5, 3}, Square{5, 4},
    Square{5, 5}, Square{5, 6}, Square{5, 7}, Square{6, 0}, Square{6, 1},
    Square{6, 2}, Square{6, 3}, Square{6, 4}, Square{6, 5}, Square{6, 6},
    Square{6, 7}, Square{7, 0}, Square{7, 1}, Square{7, 2}, Square{7, 3},
    Square{7, 4}, Square{7, 5}, Square{7, 6}, Square{7, 7},
};
inline std::map<Square, Piece *> k_pieces =
    k_board | std::views::join |
    std::views::transform(
        [](Square const &square) -> std::pair<Square, Piece *> {
            return {square, nullptr};
        }
    ) |
    std::ranges::to<std::map<Square, Piece *>>();
inline std::map<Colour, Square> k_king_pos = {
    {Colour::white, k_board[7][4]},
    {Colour::black, k_board[0][4]},
};
inline auto k_current_player = Colour::white;

#endif
