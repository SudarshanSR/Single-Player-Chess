#ifndef PIECES_H
#define PIECES_H

#include "../colour.h"

#include <set>

struct Move;
struct Square;

struct Piece {
    Colour colour;

    explicit Piece(Colour colour);

    virtual ~Piece() = default;

    [[nodiscard]] virtual auto get_moves(Square const &current_square)
        -> std::set<Move> = 0;
};

struct Pawn final : Piece {
    using Piece::Piece;

    bool moved = false;
    bool can_be_en_passanted = false;

    [[nodiscard]] auto get_moves(Square const &current_square)
        -> std::set<Move> override;
};

struct Knight final : Piece {
    using Piece::Piece;

    [[nodiscard]] auto get_moves(Square const &current_square)
        -> std::set<Move> override;
};

struct Bishop final : Piece {
    using Piece::Piece;

    [[nodiscard]] auto get_moves(Square const &current_square)
        -> std::set<Move> override;
};

struct Rook final : Piece {
    using Piece::Piece;

    bool can_castle = true;

    [[nodiscard]] auto get_moves(Square const &current_square)
        -> std::set<Move> override;
};

struct Queen final : Piece {
    using Piece::Piece;

    [[nodiscard]] auto get_moves(Square const &current_square)
        -> std::set<Move> override;
};

struct King final : Piece {
    using Piece::Piece;

    bool moved = false;

    [[nodiscard]] auto get_moves(Square const &current_square)
        -> std::set<Move> override;

    [[nodiscard]] auto is_checked() const -> bool;
};

#endif // PIECES_H
