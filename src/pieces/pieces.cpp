#include "pieces.h"

#include "../move/move.h"
#include "../vars.h"

#include <ranges>

Piece::Piece(Colour const colour) : colour(colour) {}

auto Pawn::get_moves(Square const &current_square) -> std::set<Move> {
    std::set<Move> moves;

    auto const &[rank, file] = current_square;
    std::array<Square, 8> const &current_rank = k_board[rank];

    std::array<Square, 8> const *front_rank;

    if (this->colour == Colour::white) {
        front_rank = &k_board[rank - 1];

        if (rank > 1)
            if (std::array<Square, 8> const &two_front_rank = k_board[rank - 2];
                !this->moved && !k_pieces[two_front_rank[file]])
                moves.emplace(current_square, two_front_rank[file]);
    } else {
        front_rank = &k_board[rank + 1];

        if (rank < 6)
            if (std::array<Square, 8> const &two_front_rank = k_board[rank + 2];
                !this->moved && !k_pieces[two_front_rank[file]])
                moves.emplace(current_square, two_front_rank[file]);
    }

    if (!k_pieces[front_rank->at(file)])
        moves.emplace(current_square, front_rank->at(file));

    if (file != 0) {
        Piece const *piece = k_pieces[current_rank[file - 1]];
        auto const *pawn =
            dynamic_cast<Pawn *>(k_pieces[front_rank->at(file - 1)]);

        if ((piece && piece->colour != this->colour) ||
            (pawn && pawn->can_be_en_passanted && pawn->colour != this->colour))
            moves.emplace(current_square, front_rank->at(file - 1));
    }

    if (file != 7) {
        Piece const *piece = k_pieces[current_rank[file + 1]];
        auto const *pawn =
            dynamic_cast<Pawn *>(k_pieces[front_rank->at(file + 1)]);

        if ((piece && piece->colour != this->colour) ||
            (pawn && pawn->can_be_en_passanted && pawn->colour != this->colour))
            moves.emplace(current_square, front_rank->at(file + 1));
    }

    return moves | std::views::filter([](Move const &move) -> bool {
               return move.is_valid();
           }) |
           std::ranges::to<std::set>();
}

auto Knight::get_moves(Square const &current_square) -> std::set<Move> {
    std::set<Move> moves;

    auto const &[rank, file] = current_square;

    if (rank >= 2) {
        if (file >= 1) {
            Square const &square = k_board[rank - 2][file - 1];

            if (Piece const *piece = k_pieces[square];
                !piece || piece->colour != this->colour)
                moves.emplace(current_square, square);
        }

        if (file >= 2) {
            Square const &square = k_board[rank - 1][file - 2];

            if (Piece const *piece = k_pieces[square];
                !piece || piece->colour != this->colour)
                moves.emplace(current_square, square);
        }

        if (file < 7) {
            Square const &square = k_board[rank - 2][file + 1];

            if (Piece const *piece = k_pieces[square];
                !piece || piece->colour != this->colour)
                moves.emplace(current_square, square);
        }

        if (file < 6) {
            Square const &square = k_board[rank - 1][file + 2];

            if (Piece const *piece = k_pieces[square];
                !piece || piece->colour != this->colour)
                moves.emplace(current_square, square);
        }
    }

    if (rank < 6) {
        if (file >= 1) {
            Square const &square = k_board[rank + 2][file - 1];

            if (Piece const *piece = k_pieces[square];
                !piece || piece->colour != this->colour)
                moves.emplace(current_square, square);
        }

        if (file >= 2) {
            Square const &square = k_board[rank + 1][file - 2];

            if (Piece const *piece = k_pieces[square];
                !piece || piece->colour != this->colour)
                moves.emplace(current_square, square);
        }

        if (file < 7) {
            Square const &square = k_board[rank + 2][file + 1];

            if (Piece const *piece = k_pieces[square];
                !piece || piece->colour != this->colour)
                moves.emplace(current_square, square);
        }

        if (file < 6) {
            Square const &square = k_board[rank + 1][file + 2];

            if (Piece const *piece = k_pieces[square];
                !piece || piece->colour != this->colour)
                moves.emplace(current_square, square);
        }
    }

    return moves | std::views::filter([](Move const &move) -> bool {
               return move.is_valid();
           }) |
           std::ranges::to<std::set>();
}

auto Bishop::get_moves(Square const &current_square) -> std::set<Move> {
    std::set<Move> moves;

    auto const &[rank, file] = current_square;

    File left_file = file - 1, right_file = file + 1;

    bool found_left = false, found_right = false;

    for (std::array<Square, 8> const &array :
         k_board | std::views::take(rank) | std::views::reverse) {
        if (!found_left && left_file >= 0) {
            Square const &square = array[left_file];

            if (Piece const *piece = k_pieces[square]) {
                if (piece->colour != this->colour)
                    moves.emplace(current_square, square);

                found_left = true;
            } else {
                moves.emplace(current_square, square);
            }

            --left_file;
        }

        if (!found_right && right_file < 8) {
            Square const &square = array[right_file];

            if (Piece const *piece = k_pieces[square]) {
                if (piece->colour != this->colour)
                    moves.emplace(current_square, square);

                found_right = true;
            } else {
                moves.emplace(current_square, square);
            }

            ++right_file;
        }
    }

    left_file = file - 1, right_file = file + 1;

    found_left = false, found_right = false;

    for (std::array<Square, 8> const &array :
         k_board | std::views::drop(rank + 1)) {
        if (!found_left && left_file >= 0) {
            Square const &square = array[left_file];

            if (Piece const *piece = k_pieces[square]) {
                if (piece->colour != this->colour)
                    moves.emplace(current_square, square);

                found_left = true;
            } else {
                moves.emplace(current_square, square);
            }

            --left_file;
        }

        if (!found_right && right_file < 8) {
            Square const &square = array[right_file];

            if (Piece const *piece = k_pieces[square]) {
                if (piece->colour != this->colour)
                    moves.emplace(current_square, square);

                found_right = true;
            } else {
                moves.emplace(current_square, square);
            }

            ++right_file;
        }
    }

    return moves | std::views::filter([](Move const &move) -> bool {
               return move.is_valid();
           }) |
           std::ranges::to<std::set>();
}

auto Rook::get_moves(Square const &current_square) -> std::set<Move> {
    std::set<Move> moves;

    auto const &[rank, file] = current_square;

    for (std::array<Square, 8> const &array :
         k_board | std::views::take(rank) | std::views::reverse) {
        Square const &square = array[file];

        if (Piece const *piece = k_pieces[square]) {
            if (piece->colour != this->colour)
                moves.emplace(current_square, square);

            break;
        }

        moves.emplace(current_square, square);
    }

    for (std::array<Square, 8> const &array :
         k_board | std::views::drop(rank + 1)) {
        Square const &square = array[file];

        if (Piece const *piece = k_pieces[square]) {
            if (piece->colour != this->colour)
                moves.emplace(current_square, square);

            break;
        }

        moves.emplace(current_square, square);
    }

    for (Square const &square :
         k_board[rank] | std::views::take(file) | std::views::reverse) {
        if (Piece const *piece = k_pieces[square]) {
            if (piece->colour != this->colour)
                moves.emplace(current_square, square);

            break;
        }

        moves.emplace(current_square, square);
    }

    for (Square const &square : k_board[rank] | std::views::drop(file + 1)) {
        if (Piece const *piece = k_pieces[square]) {
            if (piece->colour != this->colour)
                moves.emplace(current_square, square);

            break;
        }

        moves.emplace(current_square, square);
    }

    return moves | std::views::filter([](Move const &move) -> bool {
               return move.is_valid();
           }) |
           std::ranges::to<std::set>();
}

auto Queen::get_moves(Square const &current_square) -> std::set<Move> {
    std::set<Move> const &rank_and_files =
        Rook(this->colour).get_moves(current_square);
    std::set<Move> const &diagonals =
        Bishop(this->colour).get_moves(current_square);

    std::set<Move> moves;
    moves.insert(rank_and_files.cbegin(), rank_and_files.cend());
    moves.insert(diagonals.cbegin(), diagonals.cend());

    return moves;
}

auto King::get_moves(Square const &current_square) -> std::set<Move> {
    std::set<Move> moves;
    auto const &[opposite_king_rank, opposite_king_file] = k_king_pos[(
        this->colour == Colour::white ? Colour::black : Colour::white
    )];

    for (auto const &[x, y] : std::views::cartesian_product(
             std::array{-1, 0, 1}, std::array{-1, 0, 1}
         )) {
        if (!x && !y)
            continue;

        Rank const rank = current_square.rank + x;
        File const file = current_square.file + y;

        if (0 > rank || rank >= 8 || 0 > file || file >= 8)
            continue;

        if (std::int16_t const rank_diff = opposite_king_rank - rank,
            file_diff = opposite_king_file - file;
            rank_diff > -2 && rank_diff < 2 && file_diff > -2 && file_diff < 2)
            continue;

        Square const &square = k_board[rank][file];

        if (Piece const *piece = k_pieces[square];
            piece && piece->colour == this->colour)
            continue;

        moves.emplace(current_square, square);
    }

    if (!this->moved && !this->is_checked()) {
        Rank const rank = this->colour == Colour::white ? 7 : 0;

        if (auto const *long_rook =
                dynamic_cast<Rook *>(k_pieces[k_board[rank][0]]);
            long_rook && long_rook->can_castle && !k_pieces[k_board[rank][1]] &&
            !k_pieces[k_board[rank][2]] && !k_pieces[k_board[rank][3]] &&
            Move(current_square, k_board[rank][3]).is_valid())
            moves.emplace(current_square, k_board[rank][2]);

        if (auto const *short_rook =
                dynamic_cast<Rook *>(k_pieces[k_board[rank][7]]);
            short_rook && short_rook->can_castle &&
            !k_pieces[k_board[rank][5]] && !k_pieces[k_board[rank][6]] &&
            Move(current_square, k_board[rank][5]).is_valid())
            moves.emplace(current_square, k_board[rank][6]);
    }

    return moves | std::views::filter([](Move const &move) -> bool {
               return move.is_valid();
           }) |
           std::ranges::to<std::set>();
}

auto King::is_checked() const -> bool {
    auto const &[rank, file] = k_king_pos[this->colour];

    std::array<Square, 8> const &front_rank =
        this->colour == Colour::white ? k_board[rank - 1] : k_board[rank + 1];

    if (file != 0)
        if (Piece const *piece = k_pieces[front_rank[file - 1]];
            piece && typeid(*piece) == typeid(Pawn) &&
            piece->colour != this->colour)
            return true;

    if (file != 7)
        if (Piece const *piece = k_pieces[front_rank[file + 1]];
            piece && typeid(*piece) == typeid(Pawn) &&
            piece->colour != this->colour)
            return true;

    if (rank >= 2) {
        if (file >= 1)
            if (Piece const *piece = k_pieces[k_board[rank - 2][file - 1]];
                piece && typeid(*piece) == typeid(Knight) &&
                piece->colour != this->colour)
                return true;

        if (file >= 2)
            if (Piece const *piece = k_pieces[k_board[rank - 1][file - 2]];
                piece && typeid(*piece) == typeid(Knight) &&
                piece->colour != this->colour)
                return true;

        if (file < 7)
            if (Piece const *piece = k_pieces[k_board[rank - 2][file + 1]];
                piece && typeid(*piece) == typeid(Knight) &&
                piece->colour != this->colour)
                return true;

        if (file < 6)
            if (Piece const *piece = k_pieces[k_board[rank - 1][file + 2]];
                piece && typeid(*piece) == typeid(Knight) &&
                piece->colour != this->colour)
                return true;
    }

    if (rank < 6) {
        if (file >= 1)
            if (Piece const *piece = k_pieces[k_board[rank + 2][file - 1]];
                piece && typeid(*piece) == typeid(Knight) &&
                piece->colour != this->colour)
                return true;

        if (file >= 2)
            if (Piece const *piece = k_pieces[k_board[rank + 1][file - 2]];
                piece && typeid(*piece) == typeid(Knight) &&
                piece->colour != this->colour)
                return true;

        if (file < 7)
            if (Piece const *piece = k_pieces[k_board[rank + 2][file + 1]];
                piece && typeid(*piece) == typeid(Knight) &&
                piece->colour != this->colour)
                return true;

        if (file < 6)
            if (Piece const *piece = k_pieces[k_board[rank + 1][file + 2]];
                piece && typeid(*piece) == typeid(Knight) &&
                piece->colour != this->colour)
                return true;
    }

    File left_file = file - 1, right_file = file + 1;

    bool found_left = false, found_right = false, found_file = false;

    for (std::array<Square, 8> const &array :
         k_board | std::views::take(rank) | std::views::reverse) {
        if (!found_file)
            if (Piece const *piece = k_pieces[array[file]]) {
                if (piece->colour != this->colour &&
                    (typeid(*piece) == typeid(Rook) ||
                     typeid(*piece) == typeid(Queen)))
                    return true;

                found_file = true;
            }

        if (!found_left && left_file >= 0) {
            if (Piece const *piece = k_pieces[array[left_file]]) {
                if (piece->colour != this->colour &&
                    (typeid(*piece) == typeid(Bishop) ||
                     typeid(*piece) == typeid(Queen)))
                    return true;

                found_left = true;
            }

            --left_file;
        }

        if (!found_right && right_file < 8) {
            Square const &square = array[right_file];

            if (Piece const *piece = k_pieces[square]) {
                if (piece->colour != this->colour &&
                    (typeid(*piece) == typeid(Bishop) ||
                     typeid(*piece) == typeid(Queen)))
                    return true;

                found_right = true;
            }

            ++right_file;
        }
    }

    left_file = file - 1, right_file = file + 1;

    found_left = false, found_right = false, found_file = false;

    for (std::array<Square, 8> const &array :
         k_board | std::views::drop(rank + 1)) {
        if (!found_file)
            if (Piece const *piece = k_pieces[array[file]]) {
                if (piece->colour != this->colour &&
                    (typeid(*piece) == typeid(Rook) ||
                     typeid(*piece) == typeid(Queen)))
                    return true;

                found_file = true;
            }

        if (!found_left && left_file >= 0) {
            if (Piece const *piece = k_pieces[array[left_file]]) {
                if (piece->colour != this->colour &&
                    (typeid(*piece) == typeid(Bishop) ||
                     typeid(*piece) == typeid(Queen)))
                    return true;

                found_left = true;
            }

            --left_file;
        }

        if (!found_right && right_file < 8) {
            if (Piece const *piece = k_pieces[array[right_file]]) {
                if (piece->colour != this->colour &&
                    (typeid(*piece) == typeid(Bishop) ||
                     typeid(*piece) == typeid(Queen)))
                    return true;

                found_right = true;
            }

            ++right_file;
        }
    }

    for (Square const &square :
         k_board[rank] | std::views::take(file) | std::views::reverse) {
        Piece const *piece = k_pieces[square];

        if (!piece)
            continue;

        if (piece->colour != this->colour &&
            (typeid(*piece) == typeid(Rook) || typeid(*piece) == typeid(Queen)))
            return true;

        break;
    }

    for (Square const &square : k_board[rank] | std::views::drop(file + 1)) {
        Piece const *piece = k_pieces[square];

        if (!piece)
            continue;

        if (piece->colour != this->colour &&
            (typeid(*piece) == typeid(Rook) || typeid(*piece) == typeid(Queen)))
            return true;

        break;
    }

    return false;
}