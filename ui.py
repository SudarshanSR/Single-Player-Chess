from typing import Callable

from PySide6.QtCore import Qt, Slot
from PySide6.QtGui import QFontMetricsF
from PySide6.QtWidgets import QApplication, QPushButton, QDialog, QGridLayout, \
    QVBoxLayout, QLabel, QWidget

from colour import Colour
from piece import Pawn, Knight, Bishop, Rook, Queen, King
from square import Square
import vars


class Promote(QDialog):
    def __init__(self, colour: Colour, square: Square, button: QPushButton,
                 font_size: int, parent: QWidget):
        super().__init__(parent)

        self.setWindowFlags(
            Qt.WindowType.Window | Qt.WindowType.WindowTitleHint | Qt.WindowType.CustomizeWindowHint)

        self.setAttribute(Qt.WidgetAttribute.WA_DeleteOnClose)

        self.square: Square = square
        self.colour: Colour = colour

        if colour == Colour.black:
            queen = QPushButton("♛", self)
            rook = QPushButton("♜", self)
            bishop = QPushButton("♝", self)
            knight = QPushButton("♞", self)

            queen.clicked.connect(
                lambda _=None, button=button: self.promote(Queen, "♛", button))
            rook.clicked.connect(
                lambda _=None, button=button: self.promote(Rook, "♜", button))
            bishop.clicked.connect(
                lambda _=None, button=button: self.promote(Bishop, "♝", button))
            knight.clicked.connect(
                lambda _=None, button=button: self.promote(Knight, "♞", button))

        else:
            queen = QPushButton("♕", self)
            rook = QPushButton("♖", self)
            bishop = QPushButton("♗", self)
            knight = QPushButton("♘", self)

            queen.clicked.connect(
                lambda _=None, button=button: self.promote(Queen, "♕", button))
            rook.clicked.connect(
                lambda _=None, button=button: self.promote(Rook, "♖", button))
            bishop.clicked.connect(
                lambda _=None, button=button: self.promote(Bishop, "♗", button))
            knight.clicked.connect(
                lambda _=None, button=button: self.promote(Knight, "♘", button))

        layout = QVBoxLayout(self)

        layout.addWidget(QLabel("Promote to:"))

        for button in (queen, rook, bishop, knight):
            button.setMaximumSize(font_size, font_size)
            button.setMinimumSize(font_size, font_size)
            button.setStyleSheet("QPushButton { font : 32px; }")
            layout.addWidget(button)

    def promote(self, piece: Callable, text: str, button: QPushButton):
        vars.PIECES[self.square] = piece(self.colour)

        button.setText(text)

        self.close()


class UI(QDialog):
    def __init__(self):
        self.buttons: dict[Square, QPushButton] = {
            square: QPushButton() for row in vars.BOARD for square in row
        }

        super().__init__(None)

        self.player = QLabel("White to play")
        self.font_size = int(
            QFontMetricsF(QApplication.font()).horizontalAdvance(' ') * 16)

        board = QGridLayout()
        board.setHorizontalSpacing(0)
        board.setVerticalSpacing(0)

        self.selected_piece = None
        self.current_rank = 0
        self.current_file = 0

        for square, button in self.buttons.items():
            button.setMaximumSize(self.font_size, self.font_size)
            button.setMinimumSize(self.font_size, self.font_size)

            button.clicked.connect(
                lambda _=None, square=square: self.select_piece(square))

            rank, file = square.rank, square.file

            button.setStyleSheet(
                "QPushButton {"
                f"   background-color : {'#101010' if rank % 2 != file % 2 else '#303030'};"
                "   border : none;"
                "   font : 32px;"
                "}"
                ""
                "QPushButton:disabled {"
                f"   background-color : {'#0A0A0A' if rank % 2 != file % 2 else '#1A1A1A'};"
                "   border : none;"
                "   font : 32px;"
                "}"
            )

            board.addWidget(button, rank, file)

        self.set_board()

        new_button = QPushButton("New Game")
        new_button.clicked.connect(self.new_game)

        markings = QGridLayout()

        markings.addWidget(QLabel("1"), 0, 1, Qt.AlignmentFlag.AlignCenter)
        markings.addWidget(QLabel("2"), 0, 2, Qt.AlignmentFlag.AlignCenter)
        markings.addWidget(QLabel("3"), 0, 3, Qt.AlignmentFlag.AlignCenter)
        markings.addWidget(QLabel("4"), 0, 4, Qt.AlignmentFlag.AlignCenter)
        markings.addWidget(QLabel("5"), 0, 5, Qt.AlignmentFlag.AlignCenter)
        markings.addWidget(QLabel("6"), 0, 6, Qt.AlignmentFlag.AlignCenter)
        markings.addWidget(QLabel("7"), 0, 7, Qt.AlignmentFlag.AlignCenter)
        markings.addWidget(QLabel("8"), 0, 8, Qt.AlignmentFlag.AlignCenter)

        markings.addWidget(QLabel("h"), 1, 0, Qt.AlignmentFlag.AlignCenter)
        markings.addWidget(QLabel("g"), 2, 0, Qt.AlignmentFlag.AlignCenter)
        markings.addWidget(QLabel("f"), 3, 0, Qt.AlignmentFlag.AlignCenter)
        markings.addWidget(QLabel("e"), 4, 0, Qt.AlignmentFlag.AlignCenter)
        markings.addWidget(QLabel("d"), 5, 0, Qt.AlignmentFlag.AlignCenter)
        markings.addWidget(QLabel("c"), 6, 0, Qt.AlignmentFlag.AlignCenter)
        markings.addWidget(QLabel("b"), 7, 0, Qt.AlignmentFlag.AlignCenter)
        markings.addWidget(QLabel("a"), 8, 0, Qt.AlignmentFlag.AlignCenter)

        markings.addWidget(QLabel("h"), 1, 9, Qt.AlignmentFlag.AlignCenter)
        markings.addWidget(QLabel("g"), 2, 9, Qt.AlignmentFlag.AlignCenter)
        markings.addWidget(QLabel("f"), 3, 9, Qt.AlignmentFlag.AlignCenter)
        markings.addWidget(QLabel("e"), 4, 9, Qt.AlignmentFlag.AlignCenter)
        markings.addWidget(QLabel("d"), 5, 9, Qt.AlignmentFlag.AlignCenter)
        markings.addWidget(QLabel("c"), 6, 9, Qt.AlignmentFlag.AlignCenter)
        markings.addWidget(QLabel("b"), 7, 9, Qt.AlignmentFlag.AlignCenter)
        markings.addWidget(QLabel("a"), 8, 9, Qt.AlignmentFlag.AlignCenter)

        markings.addWidget(QLabel("1"), 9, 1, Qt.AlignmentFlag.AlignCenter)
        markings.addWidget(QLabel("2"), 9, 2, Qt.AlignmentFlag.AlignCenter)
        markings.addWidget(QLabel("3"), 9, 3, Qt.AlignmentFlag.AlignCenter)
        markings.addWidget(QLabel("4"), 9, 4, Qt.AlignmentFlag.AlignCenter)
        markings.addWidget(QLabel("5"), 9, 5, Qt.AlignmentFlag.AlignCenter)
        markings.addWidget(QLabel("6"), 9, 6, Qt.AlignmentFlag.AlignCenter)
        markings.addWidget(QLabel("7"), 9, 7, Qt.AlignmentFlag.AlignCenter)
        markings.addWidget(QLabel("8"), 9, 8, Qt.AlignmentFlag.AlignCenter)

        markings.addLayout(board, 1, 1, 8, 8)

        layout = QVBoxLayout(self)
        layout.setSizeConstraint(QGridLayout.SizeConstraint.SetFixedSize)
        layout.addWidget(self.player, Qt.AlignmentFlag.AlignCenter)
        layout.addLayout(markings)
        layout.addWidget(new_button)

    def set_board(self):
        vars.PIECES[vars.BOARD[0][0]] = Rook(Colour.black)
        vars.PIECES[vars.BOARD[0][1]] = Knight(Colour.black)
        vars.PIECES[vars.BOARD[0][2]] = Bishop(Colour.black)
        vars.PIECES[vars.BOARD[0][3]] = Queen(Colour.black)
        vars.PIECES[vars.BOARD[0][4]] = King(Colour.black)
        vars.PIECES[vars.BOARD[0][5]] = Bishop(Colour.black)
        vars.PIECES[vars.BOARD[0][6]] = Knight(Colour.black)
        vars.PIECES[vars.BOARD[0][7]] = Rook(Colour.black)

        vars.PIECES[vars.BOARD[1][0]] = Pawn(Colour.black)
        vars.PIECES[vars.BOARD[1][1]] = Pawn(Colour.black)
        vars.PIECES[vars.BOARD[1][2]] = Pawn(Colour.black)
        vars.PIECES[vars.BOARD[1][3]] = Pawn(Colour.black)
        vars.PIECES[vars.BOARD[1][4]] = Pawn(Colour.black)
        vars.PIECES[vars.BOARD[1][5]] = Pawn(Colour.black)
        vars.PIECES[vars.BOARD[1][6]] = Pawn(Colour.black)
        vars.PIECES[vars.BOARD[1][7]] = Pawn(Colour.black)

        vars.PIECES[vars.BOARD[6][0]] = Pawn(Colour.white)
        vars.PIECES[vars.BOARD[6][1]] = Pawn(Colour.white)
        vars.PIECES[vars.BOARD[6][2]] = Pawn(Colour.white)
        vars.PIECES[vars.BOARD[6][3]] = Pawn(Colour.white)
        vars.PIECES[vars.BOARD[6][4]] = Pawn(Colour.white)
        vars.PIECES[vars.BOARD[6][5]] = Pawn(Colour.white)
        vars.PIECES[vars.BOARD[6][6]] = Pawn(Colour.white)
        vars.PIECES[vars.BOARD[6][7]] = Pawn(Colour.white)

        vars.PIECES[vars.BOARD[7][0]] = Rook(Colour.white)
        vars.PIECES[vars.BOARD[7][1]] = Knight(Colour.white)
        vars.PIECES[vars.BOARD[7][2]] = Bishop(Colour.white)
        vars.PIECES[vars.BOARD[7][3]] = Queen(Colour.white)
        vars.PIECES[vars.BOARD[7][4]] = King(Colour.white)
        vars.PIECES[vars.BOARD[7][5]] = Bishop(Colour.white)
        vars.PIECES[vars.BOARD[7][6]] = Knight(Colour.white)
        vars.PIECES[vars.BOARD[7][7]] = Rook(Colour.white)

        self.buttons[vars.BOARD[0][0]].setText("♜")
        self.buttons[vars.BOARD[0][1]].setText("♞")
        self.buttons[vars.BOARD[0][2]].setText("♝")
        self.buttons[vars.BOARD[0][3]].setText("♛")
        self.buttons[vars.BOARD[0][4]].setText("♚")
        self.buttons[vars.BOARD[0][5]].setText("♝")
        self.buttons[vars.BOARD[0][6]].setText("♞")
        self.buttons[vars.BOARD[0][7]].setText("♜")

        self.buttons[vars.BOARD[1][0]].setText("♟︎")
        self.buttons[vars.BOARD[1][1]].setText("♟︎")
        self.buttons[vars.BOARD[1][2]].setText("♟︎")
        self.buttons[vars.BOARD[1][3]].setText("♟︎")
        self.buttons[vars.BOARD[1][4]].setText("♟︎")
        self.buttons[vars.BOARD[1][5]].setText("♟︎")
        self.buttons[vars.BOARD[1][6]].setText("♟︎")
        self.buttons[vars.BOARD[1][7]].setText("♟︎")

        self.buttons[vars.BOARD[6][0]].setText("♙")
        self.buttons[vars.BOARD[6][1]].setText("♙")
        self.buttons[vars.BOARD[6][2]].setText("♙")
        self.buttons[vars.BOARD[6][3]].setText("♙")
        self.buttons[vars.BOARD[6][4]].setText("♙")
        self.buttons[vars.BOARD[6][5]].setText("♙")
        self.buttons[vars.BOARD[6][6]].setText("♙")
        self.buttons[vars.BOARD[6][7]].setText("♙")

        self.buttons[vars.BOARD[7][0]].setText("♖")
        self.buttons[vars.BOARD[7][1]].setText("♘")
        self.buttons[vars.BOARD[7][2]].setText("♗")
        self.buttons[vars.BOARD[7][3]].setText("♕")
        self.buttons[vars.BOARD[7][4]].setText("♔")
        self.buttons[vars.BOARD[7][5]].setText("♗")
        self.buttons[vars.BOARD[7][6]].setText("♘")
        self.buttons[vars.BOARD[7][7]].setText("♖")

        for square, piece in vars.PIECES.items():
            self.buttons[square].setEnabled(
                piece is not None and piece.colour == vars.CURRENT_PLAYER)

    @Slot()
    def select_piece(self, square: Square):
        row, col = square.rank, square.file

        if self.selected_piece is None:
            self.selected_piece = vars.PIECES[square]

            if self.selected_piece is None:
                return

            self.current_rank = row
            self.current_file = col

            moves = self.selected_piece.get_moves(square)

            for button in self.buttons.values():
                button.setEnabled(False)

            for move in moves:
                self.buttons[move.end].setEnabled(True)

            self.buttons[square].setEnabled(True)

            return

        if self.current_rank != row or self.current_file != col:
            new_pos = vars.BOARD[self.current_rank][self.current_file]
            vars.PIECES[square], vars.PIECES[new_pos] = vars.PIECES[
                new_pos], vars.PIECES[square]

            next_button = self.buttons[vars.BOARD[row][col]]
            current_button = self.buttons[new_pos]

            text: str = next_button.text()

            if text != "":
                text = ""
                vars.PIECES[new_pos] = None

            next_button.setText(current_button.text())
            current_button.setText(text)

            colour: Colour = self.selected_piece.colour

            if vars.CURRENT_PLAYER == Colour.white:
                vars.CURRENT_PLAYER = Colour.black
                self.player.setText("Black to play")

            else:
                vars.CURRENT_PLAYER = Colour.white
                self.player.setText("White to play")

            for piece in vars.PIECES.values():
                if isinstance(piece, Pawn) and piece.colour == colour:
                    piece.can_be_en_passanted = False

            if isinstance(self.selected_piece, Pawn):
                self.selected_piece.moved = True

                if row - self.current_rank in {-2, 2}:
                    self.selected_piece.can_be_en_passanted = True

                elif (colour == Colour.white and row == 0) or (
                        colour == Colour.black and row == 7):
                    promotion_window = Promote(colour, square,
                                               self.buttons[square],
                                               self.font_size, self)

                    promotion_window.exec()

                elif (
                        self.current_file > 0 and col == self.current_file - 1) or \
                        (
                                self.current_file < 7 and col == self.current_file + 1):
                    piece = vars.PIECES[vars.BOARD[self.current_rank][col]]

                    if (isinstance(piece, Pawn) and
                            piece.colour != self.selected_piece.colour and
                            piece.can_be_en_passanted):
                        vars.PIECES[
                            vars.BOARD[self.current_rank][col]] = None
                        self.buttons[
                            vars.BOARD[self.current_rank][col]].setText(
                            "")

            elif isinstance(self.selected_piece, King):
                vars.KING_POS[colour] = vars.BOARD[row][col]

                self.selected_piece.moved = True

                for piece in vars.PIECES.values():
                    if isinstance(piece, Rook) and piece.colour == colour:
                        piece.can_castle = False

                if col - self.current_file in {-2, 2}:
                    rank = 7 if colour == Colour.white else 0

                    if col == 2:
                        square = vars.BOARD[rank][0]
                        end = vars.BOARD[rank][3]

                    else:
                        square = vars.BOARD[rank][7]
                        end = vars.BOARD[rank][5]

                    vars.PIECES[square], vars.PIECES[end] = vars.PIECES[
                        end], vars.PIECES[square]

                    self.buttons[end].setText(self.buttons[square].text())
                    self.buttons[square].setText("")

            elif self.selected_piece.type == "Rook":
                self.selected_piece.can_castle = False

            for square, piece in vars.PIECES.items():
                if piece is None or piece.colour != vars.CURRENT_PLAYER:
                    continue

                if len(piece.get_moves(square)):
                    break

            else:
                if vars.PIECES[
                    vars.KING_POS[vars.CURRENT_PLAYER]].is_checked():
                    self.player.setText(
                        f"Checkmate! {'White' if vars.CURRENT_PLAYER == 'Black' else 'Black'} wins!")

                else:
                    self.player.setText("Stalement")

                for button in self.buttons.values():
                    button.setEnabled(False)

                return

        self.current_rank = 0
        self.current_file = 0
        self.selected_piece = None

        for square, piece in vars.PIECES.items():
            self.buttons[square].setEnabled(
                piece is not None and piece.colour == vars.CURRENT_PLAYER)

    @Slot()
    def new_game(self):
        for square in vars.PIECES.keys():
            vars.PIECES[square] = None

        for button in self.buttons.values():
            button.setText("")

        vars.KING_POS[Colour.white] = vars.BOARD[7][4]
        vars.KING_POS[Colour.black] = vars.BOARD[0][4]

        vars.CURRENT_PLAYER = Colour.white

        self.set_board()
