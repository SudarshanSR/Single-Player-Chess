import abc
import itertools
from dataclasses import dataclass
from typing import Optional, final, override

import vars
from colour import Colour
from move import Move
from square import File, Rank, Square


@dataclass
class Piece(abc.ABC):
    colour: Colour

    @abc.abstractmethod
    def get_moves(self, current_square: Square) -> set[Move]:
        pass


@final
@dataclass
class Pawn(Piece):
    moved: bool = False
    can_be_en_passanted: bool = False

    def __init__(self, colour: Colour):
        super().__init__(colour)

    @override
    def get_moves(self, current_square: Square) -> set[Move]:
        moves: set[Move] = set()

        rank, file = current_square.rank, current_square.file

        current_rank: list[Square] = vars.BOARD[rank]

        front_rank: list[Square]

        if self.colour == Colour.white:
            front_rank = vars.BOARD[rank - 1]

            if rank > 1:
                two_front_rank: list[Square] = vars.BOARD[rank - 2]

                if not self.moved and vars.PIECES[two_front_rank[file]] is None:
                    moves.add(Move(current_square, two_front_rank[file]))

        else:
            front_rank = vars.BOARD[rank + 1]

            if rank < 6:
                two_front_rank: list[Square] = vars.BOARD[rank + 2]

                if not self.moved and vars.PIECES[two_front_rank[file]] is None:
                    moves.add(Move(current_square, two_front_rank[file]))

        if vars.PIECES[front_rank[file]] is None:
            moves.add(Move(current_square, front_rank[file]))

        if file != 0:
            piece: Optional[Piece] = vars.PIECES[front_rank[file - 1]]
            pawn: Optional[Pawn] = vars.PIECES[current_rank[file - 1]]

            if (
                    (piece is not None and piece.colour != self.colour) or
                    (pawn is not None and
                     isinstance(pawn, Pawn) and
                     pawn.can_be_en_passanted and
                     pawn.colour != self.colour)
            ):
                moves.add(Move(current_square, front_rank[file - 1]))

        if file != 7:
            piece: Optional[Piece] = vars.PIECES[front_rank[file + 1]]
            pawn: Optional[Pawn] = vars.PIECES[current_rank[file + 1]]

            if (
                    (piece is not None and piece.colour != self.colour) or
                    (pawn is not None and
                     isinstance(pawn, Pawn) and
                     pawn.can_be_en_passanted and
                     pawn.colour != self.colour)
            ):
                moves.add(Move(current_square, front_rank[file + 1]))

        return {move for move in moves if move.is_valid()}


@final
@dataclass
class Knight(Piece):
    def __init__(self, colour: Colour):
        super().__init__(colour)

    @override
    def get_moves(self, current_pos: Square) -> set[Move]:
        moves: set[Move] = set()

        rank, file = current_pos.rank, current_pos.file

        if rank >= 2:
            if file >= 1:
                square: Square = vars.BOARD[rank - 2][file - 1]

                piece: Optional[Piece] = vars.PIECES[square]

                if piece is None or piece.colour != self.colour:
                    moves.add(Move(current_pos, square))

            if file >= 2:
                square = vars.BOARD[rank - 1][file - 2]

                piece: Optional[Piece] = vars.PIECES[square]

                if piece is None or piece.colour != self.colour:
                    moves.add(Move(current_pos, square))

            if file < 7:
                square = vars.BOARD[rank - 2][file + 1]

                piece: Optional[Piece] = vars.PIECES[square]

                if piece is None or piece.colour != self.colour:
                    moves.add(Move(current_pos, square))

            if file < 6:
                square = vars.BOARD[rank - 1][file + 2]

                piece: Optional[Piece] = vars.PIECES[square]

                if piece is None or piece.colour != self.colour:
                    moves.add(Move(current_pos, square))

        if rank < 6:
            if file >= 1:
                square = vars.BOARD[rank + 2][file - 1]

                piece: Optional[Piece] = vars.PIECES[square]

                if piece is None or piece.colour != self.colour:
                    moves.add(Move(current_pos, square))

            if file >= 2:
                square = vars.BOARD[rank + 1][file - 2]

                piece: Optional[Piece] = vars.PIECES[square]

                if piece is None or piece.colour != self.colour:
                    moves.add(Move(current_pos, square))

            if file < 7:
                square = vars.BOARD[rank + 2][file + 1]

                piece: Optional[Piece] = vars.PIECES[square]

                if piece is None or piece.colour != self.colour:
                    moves.add(Move(current_pos, square))

            if file < 6:
                square = vars.BOARD[rank + 1][file + 2]

                piece: Optional[Piece] = vars.PIECES[square]

                if piece is None or piece.colour != self.colour:
                    moves.add(Move(current_pos, square))

        return {move for move in moves if move.is_valid()}


@final
@dataclass
class Bishop(Piece):
    def __init__(self, colour: Colour):
        super().__init__(colour)

    @override
    def get_moves(self, current_pos: Square) -> set[Move]:
        moves: set[Move] = set()

        rank, file = current_pos.rank, current_pos.file

        file_left: File = file - 1
        file_right: File = file + 1

        left_found: bool = False
        right_found: bool = False

        array: list[Square]
        for array in vars.BOARD[:rank][::-1]:
            if not left_found and file_left >= 0:
                left_square: Square = array[file_left]

                left_piece: Optional[Piece] = vars.PIECES[left_square]

                if left_piece is not None:
                    if left_piece.colour != self.colour:
                        moves.add(Move(current_pos, left_square))

                    left_found = True

                else:
                    moves.add(Move(current_pos, left_square))

                file_left -= 1

            if not right_found and file_right < 8:
                right_square: Square = array[file_right]

                right_piece: Optional[Piece] = vars.PIECES[right_square]

                if right_piece is not None:
                    if right_piece.colour != self.colour:
                        moves.add(Move(current_pos, right_square))

                    right_found = True

                else:
                    moves.add(Move(current_pos, right_square))

                file_right += 1

        file_left = file - 1
        file_right = file + 1

        left_found = False
        right_found = False

        array: list[Square]
        for array in vars.BOARD[rank + 1:]:
            if not left_found and file_left >= 0:
                left_square: Square = array[file_left]

                left_piece: Optional[Piece] = vars.PIECES[left_square]

                if left_piece is not None:
                    if left_piece.colour != self.colour:
                        moves.add(Move(current_pos, left_square))

                    left_found = True

                else:
                    moves.add(Move(current_pos, left_square))

                file_left -= 1

            if not right_found and file_right < 8:
                right_square: Square = array[file_right]

                right_piece: Optional[Piece] = vars.PIECES[right_square]

                if right_piece is not None:
                    if right_piece.colour != self.colour:
                        moves.add(Move(current_pos, right_square))

                    right_found = True

                else:
                    moves.add(Move(current_pos, right_square))

                file_right += 1

        return {move for move in moves if move.is_valid()}


@final
@dataclass
class Rook(Piece):
    can_castle: bool = True

    def __init__(self, colour: Colour):
        super().__init__(colour)

    @override
    def get_moves(self, current_pos: Square) -> set[Move]:
        moves: set[Move] = set()

        rank, file = current_pos.rank, current_pos.file

        array: list[Square]
        for array in vars.BOARD[:rank][::-1]:
            square: Square = array[file]

            piece: Optional[Piece] = vars.PIECES[square]

            if piece is not None:
                if piece.colour != self.colour:
                    moves.add(Move(current_pos, square))

                break

            moves.add(Move(current_pos, square))

        array: list[Square]
        for array in vars.BOARD[rank + 1:]:
            square: Square = array[file]

            piece: Optional[Piece] = vars.PIECES[square]

            if piece is not None:
                if piece.colour != self.colour:
                    moves.add(Move(current_pos, square))

                break

            moves.add(Move(current_pos, square))

        square: Square
        for square in vars.BOARD[rank][:file][::-1]:
            piece: Optional[Piece] = vars.PIECES[square]

            if piece is not None:
                if self.colour != piece.colour:
                    moves.add(Move(current_pos, square))

                break

            moves.add(Move(current_pos, square))

        square: Square
        for square in vars.BOARD[rank][file + 1:]:
            piece: Optional[Piece] = vars.PIECES[square]

            if piece is not None:
                if piece.colour != self.colour:
                    moves.add(Move(current_pos, square))

                break

            moves.add(Move(current_pos, square))

        return {move for move in moves if move.is_valid()}


@final
@dataclass
class Queen(Piece):
    def __init__(self, colour: Colour):
        super().__init__(colour)

    @override
    def get_moves(self, current_pos) -> set[Move]:
        return Rook(self.colour).get_moves(current_pos).union(
            Bishop(self.colour).get_moves(current_pos))


@final
@dataclass
class King(Piece):
    moved: bool = False

    def __init__(self, colour: Colour):
        super().__init__(colour)

    @override
    def get_moves(self, current_pos: Square) -> set[Move]:
        moves: set[Move] = set()

        king_pos: Square = (vars.KING_POS[Colour.black]
                            if self.colour == Colour.white
                            else vars.KING_POS[Colour.white])

        for x, y in itertools.product((-1, 0, 1), (-1, 0, 1)):
            if x == y == 0:
                continue

            rank: Rank = current_pos.rank + x
            file: File = current_pos.file + y

            if (0 > rank or rank >= 8) or (0 > file or file >= 8):
                continue

            rank_diff: int = king_pos.rank - rank
            file_diff: int = king_pos.file - file

            if (-2 < rank_diff < 2) and (-2 < file_diff < 2):
                continue

            square: Square = vars.BOARD[rank][file]
            piece: Optional[Piece] = vars.PIECES[square]

            if piece is not None and piece.colour == self.colour:
                continue

            moves.add(Move(current_pos, square))

        if not self.moved and not self.is_checked():
            rank: Rank = 7 if self.colour == Colour.white else 0

            long_rook: Optional[Rook] = vars.PIECES[vars.BOARD[rank][0]]

            if (long_rook is not None and
                    isinstance(long_rook, Rook) and
                    long_rook.can_castle and
                    vars.PIECES[vars.BOARD[rank][1]] is None and
                    vars.PIECES[vars.BOARD[rank][2]] is None and
                    vars.PIECES[vars.BOARD[rank][3]] is None and
                    Move(current_pos, vars.BOARD[rank][3]).is_valid()):
                moves.add(Move(current_pos, vars.BOARD[rank][2]))

            short_rook: Optional[Rook] = vars.PIECES[vars.BOARD[rank][7]]

            if (short_rook is not None and
                    isinstance(short_rook, Rook) and
                    short_rook.can_castle and
                    vars.PIECES[vars.BOARD[rank][5]] is None and
                    vars.PIECES[vars.BOARD[rank][6]] is None and
                    Move(current_pos, vars.BOARD[rank][5]).is_valid()):
                moves.add(Move(current_pos, vars.BOARD[rank][6]))

        return {move for move in moves if move.is_valid()}

    def is_checked(self) -> bool:
        current_pos: Square = vars.KING_POS[self.colour]

        rank, file = current_pos.rank, current_pos.file

        front_rank = (
            vars.BOARD[rank - 1] if self.colour == Colour.white else vars.BOARD[
                rank + 1]
        )

        if file != 0:
            pawn: Optional[Pawn] = vars.PIECES[front_rank[file - 1]]

            if pawn is not None and isinstance(pawn, Pawn) and pawn.colour != self.colour:
                return True

        if file != 7:
            pawn: Optional[Pawn] = vars.PIECES[front_rank[file + 1]]

            if pawn is not None and isinstance(pawn, Pawn) and pawn.colour != self.colour:
                return True

        if rank >= 2:
            if file >= 1:
                knight: Optional[Knight] = vars.PIECES[
                    vars.BOARD[rank - 2][file - 1]]

                if knight is not None and isinstance(knight, Knight) and knight.colour != self.colour:
                    return True

            if file >= 2:
                knight: Optional[Knight] = vars.PIECES[
                    vars.BOARD[rank - 1][file - 2]]

                if knight is not None and isinstance(knight, Knight) and knight.colour != self.colour:
                    return True

            if file < 7:
                knight: Optional[Knight] = vars.PIECES[
                    vars.BOARD[rank - 2][file + 1]]

                if knight is not None and isinstance(knight, Knight) and knight.colour != self.colour:
                    return True

            if file < 6:
                knight: Optional[Knight] = vars.PIECES[
                    vars.BOARD[rank - 1][file + 2]]

                if knight is not None and isinstance(knight, Knight) and knight.colour != self.colour:
                    return True

        if rank < 6:
            if file >= 1:
                knight: Optional[Knight] = vars.PIECES[
                    vars.BOARD[rank + 2][file - 1]]

                if knight is not None and isinstance(knight, Knight) and knight.colour != self.colour:
                    return True

            if file >= 2:
                knight: Optional[Knight] = vars.PIECES[
                    vars.BOARD[rank + 1][file - 2]]

                if knight is not None and isinstance(knight, Knight) and knight.colour != self.colour:
                    return True

            if file < 7:
                knight: Optional[Knight] = vars.PIECES[
                    vars.BOARD[rank + 2][file + 1]]

                if knight is not None and isinstance(knight, Knight) and knight.colour != self.colour:
                    return True

            if file < 6:
                knight: Optional[Knight] = vars.PIECES[
                    vars.BOARD[rank + 1][file + 2]]

                if knight is not None and isinstance(knight, Knight) and knight.colour != self.colour:
                    return True

        file_left: File = file - 1
        file_right: File = file + 1

        left_found: bool = False
        right_found: bool = False
        file_found: bool = False

        array: list[Square]
        for array in vars.BOARD[:rank][::-1]:
            if not file_found:
                piece: Optional[Piece] = vars.PIECES[array[file]]

                if piece is not None:
                    if piece.colour != self.colour and (
                            isinstance(piece, Rook) or isinstance(piece, Queen)):
                        return True

                    file_found = True

            if not left_found and file_left >= 0:
                piece: Optional[Piece] = vars.PIECES[array[file_left]]

                if piece is not None:
                    if piece.colour != self.colour and (
                            isinstance(piece, Bishop) or isinstance(piece, Queen)):
                        return True

                    left_found = True

                file_left -= 1

            if not right_found and file_right < 8:
                piece: Optional[Piece] = vars.PIECES[array[file_right]]

                if piece is not None:
                    if piece.colour != self.colour and (
                            isinstance(piece, Bishop) or isinstance(piece, Queen)):
                        return True

                    right_found = True

                file_right += 1

        file_left = file - 1
        file_right = file + 1

        left_found = False
        right_found = False
        file_found = False

        array: list[Square]
        for array in vars.BOARD[rank + 1:]:
            if not file_found:
                piece: Optional[Piece] = vars.PIECES[array[file]]

                if piece is not None:
                    if piece.colour != self.colour and (
                            isinstance(piece, Rook) or isinstance(piece, Queen)):
                        return True

                    file_found = True

            if not left_found and file_left >= 0:
                piece: Optional[Piece] = vars.PIECES[array[file_left]]

                if piece is not None:
                    if piece.colour != self.colour and (
                            isinstance(piece, Bishop) or isinstance(piece, Queen)):
                        return True

                    left_found = True

                file_left -= 1

            if not right_found and file_right < 8:
                piece: Optional[Piece] = vars.PIECES[array[file_right]]

                if piece is not None:
                    if piece.colour != self.colour and (
                            isinstance(piece, Bishop) or isinstance(piece, Queen)):
                        return True

                    right_found = True

                file_right += 1

        square: Square
        for square in vars.BOARD[rank][:file][::-1]:
            piece: Optional[Piece] = vars.PIECES[square]

            if piece is None:
                continue

            if self.colour != piece.colour and (
                    isinstance(piece, Rook) or isinstance(piece, Queen)):
                return True

            break

        square: Square
        for square in vars.BOARD[rank][file + 1:]:
            piece: Optional[Piece] = vars.PIECES[square]

            if piece is None:
                continue

            if piece.colour != self.colour and (
                    isinstance(piece, Rook) or isinstance(piece, Queen)):
                return True

            break

        return False
