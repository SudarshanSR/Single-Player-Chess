#include "move.h"

#include "../pieces/pieces.h"
#include "../vars.h"

auto Move::is_valid() const -> bool {
    Piece *start_piece = k_pieces[this->start],
          *end_piece = k_pieces[this->end];

    k_pieces[this->start] = nullptr;
    k_pieces[this->end] = start_piece;

    Square const king_pos = k_king_pos[k_current_player];

    if (typeid(*start_piece) == typeid(King))
        k_king_pos[k_current_player] = this->end;

    bool const valid =
        !dynamic_cast<King *>(k_pieces[k_king_pos[k_current_player]])
             ->is_checked();

    k_pieces[this->start] = start_piece;
    k_pieces[this->end] = end_piece;

    k_king_pos[k_current_player] = king_pos;

    return valid;
}