from dataclasses import dataclass
from typing import Optional

import piece
import square
import vars


@dataclass(frozen=True)
class Move:
    start: square.Square
    end: square.Square

    def is_valid(self) -> bool:
        start_piece: Optional[piece.Piece] = vars.PIECES[self.start]
        end_piece: Optional[piece.Piece] = vars.PIECES[self.end]
        king_pos = vars.KING_POS[vars.CURRENT_PLAYER]

        vars.PIECES[self.start], vars.PIECES[self.end] = (
            None, vars.PIECES[self.start]
        )

        if isinstance(start_piece, piece.King):
            vars.KING_POS[vars.CURRENT_PLAYER] = self.end

        valid: bool = not (
            vars.PIECES[vars.KING_POS[vars.CURRENT_PLAYER]].is_checked()
        )

        vars.PIECES[self.start], vars.PIECES[self.end] = start_piece, end_piece
        vars.KING_POS[vars.CURRENT_PLAYER] = king_pos

        return valid
