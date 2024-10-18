from typing import Optional, NewType

import square
import colour

Piece = NewType("Piece", None)

BOARD: list[list[square.Square]] = [[square.Square(rank, file) for file in range(8)] for rank in range(8)]
PIECES: dict[square.Square, Optional[Piece]] = {
    square: None for row in BOARD for square in row
}
KING_POS: dict[colour.Colour, square.Square] = {colour.Colour.white: BOARD[7][4], colour.Colour.black: BOARD[0][4]}
CURRENT_PLAYER: colour.Colour = colour.Colour.white
