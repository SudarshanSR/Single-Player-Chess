#include "window.h"

#include "../move/move.h"
#include "../pieces/pieces.h"
#include "../vars.h"
#include "promotion.h"
#include "record.h"

#include <ranges>

#include <qboxlayout.h>
#include <qgridlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>

MainWindow::MainWindow(QWidget *parent) : QDialog(parent) {
    this->buttons =
        k_board | std::views::join |
        std::views::transform(
            [](Square const &square) -> std::pair<Square, QPushButton *> {
                return {square, new QPushButton};
            }
        ) |
        std::ranges::to<std::map>();

    this->player = new QLabel("White to play", this);

    auto *board = new QGridLayout;
    board->setHorizontalSpacing(0);
    board->setVerticalSpacing(0);

    for (auto const &[square, button] : this->buttons) {
        button->setMaximumSize(this->k_font_size, this->k_font_size);
        button->setMinimumSize(this->k_font_size, this->k_font_size);

        connect(button, &QPushButton::clicked, [this, square]() -> void {
            this->selectPiece(square);
        });

        auto const &[rank, file] = square;

        if (rank % 2 != file % 2) {
            button->setStyleSheet(
                "QPushButton {"
                "   background-color : #101010;"
                "   border : none;"
                "   font : 32px;"
                "}"
                ""
                "QPushButton:disabled {"
                "   background-color : #0A0A0A;"
                "   border : none;"
                "   font : 32px;"
                "}"
            );
        } else {
            button->setStyleSheet(
                "QPushButton {"
                "   background-color : #303030;"
                "   border : none;"
                "   font : 32px;"
                "}"
                ""
                "QPushButton:disabled {"
                "   background-color : #1A1A1A;"
                "   border : none;"
                "   font : 32px;"
                "}"
            );
        }

        board->addWidget(button, rank, file);
    }

    this->setBoard();

    auto *markings = new QGridLayout;

    markings->addWidget(new QLabel("a"), 0, 1, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("b"), 0, 2, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("c"), 0, 3, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("d"), 0, 4, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("e"), 0, 5, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("f"), 0, 6, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("g"), 0, 7, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("h"), 0, 8, Qt::AlignmentFlag::AlignCenter);

    markings->addWidget(new QLabel("8"), 1, 0, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("7"), 2, 0, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("6"), 3, 0, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("5"), 4, 0, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("4"), 5, 0, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("3"), 6, 0, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("2"), 7, 0, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("1"), 8, 0, Qt::AlignmentFlag::AlignCenter);

    markings->addWidget(new QLabel("8"), 1, 9, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("7"), 2, 9, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("6"), 3, 9, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("5"), 4, 9, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("4"), 5, 9, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("3"), 6, 9, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("2"), 7, 9, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("1"), 8, 9, Qt::AlignmentFlag::AlignCenter);

    markings->addWidget(new QLabel("a"), 9, 1, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("b"), 9, 2, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("c"), 9, 3, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("d"), 9, 4, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("e"), 9, 5, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("f"), 9, 6, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("g"), 9, 7, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("h"), 9, 8, Qt::AlignmentFlag::AlignCenter);

    markings->addLayout(board, 1, 1, 8, 8);

    auto *newGame = new QPushButton("New Game", this);
    connect(newGame, &QPushButton::clicked, [this]() -> void {
        this->newGame();
    });

    this->record = new MoveRecord(this);

    auto *layout = new QGridLayout(this);
    layout->setSizeConstraint(QGridLayout::SizeConstraint::SetFixedSize);
    layout->addWidget(this->player, 0, 0, Qt::AlignmentFlag::AlignCenter);
    layout->addWidget(
        new QLabel("Moves"), 0, 1, Qt::AlignmentFlag::AlignCenter
    );
    layout->addLayout(markings, 1, 0);
    layout->addWidget(this->record, 1, 1);
    layout->addWidget(newGame, 2, 0);
}

void MainWindow::setBoard() {
    for (Piece const *piece : k_pieces | std::views::values)
        delete piece;

    k_pieces[k_board[0][0]] = new Rook(Colour::black);
    k_pieces[k_board[0][1]] = new Knight(Colour::black);
    k_pieces[k_board[0][2]] = new Bishop(Colour::black);
    k_pieces[k_board[0][3]] = new Queen(Colour::black);
    k_pieces[k_board[0][4]] = new King(Colour::black);
    k_pieces[k_board[0][5]] = new Bishop(Colour::black);
    k_pieces[k_board[0][6]] = new Knight(Colour::black);
    k_pieces[k_board[0][7]] = new Rook(Colour::black);

    for (std::size_t index = 0; index < 8; ++index) {
        k_pieces[k_board[1][index]] = new Pawn(Colour::black);
        k_pieces[k_board[6][index]] = new Pawn(Colour::white);

        this->buttons[k_board[1][index]]->setText("♟︎");
        this->buttons[k_board[6][index]]->setText("♙");
    }

    k_pieces[k_board[7][0]] = new Rook(Colour::white);
    k_pieces[k_board[7][1]] = new Knight(Colour::white);
    k_pieces[k_board[7][2]] = new Bishop(Colour::white);
    k_pieces[k_board[7][3]] = new Queen(Colour::white);
    k_pieces[k_board[7][4]] = new King(Colour::white);
    k_pieces[k_board[7][5]] = new Bishop(Colour::white);
    k_pieces[k_board[7][6]] = new Knight(Colour::white);
    k_pieces[k_board[7][7]] = new Rook(Colour::white);

    this->buttons[k_board[0][0]]->setText("♜");
    this->buttons[k_board[0][1]]->setText("♞");
    this->buttons[k_board[0][2]]->setText("♝");
    this->buttons[k_board[0][3]]->setText("♛");
    this->buttons[k_board[0][4]]->setText("♚");
    this->buttons[k_board[0][5]]->setText("♝");
    this->buttons[k_board[0][6]]->setText("♞");
    this->buttons[k_board[0][7]]->setText("♜");

    this->buttons[k_board[7][0]]->setText("♖");
    this->buttons[k_board[7][1]]->setText("♘");
    this->buttons[k_board[7][2]]->setText("♗");
    this->buttons[k_board[7][3]]->setText("♕");
    this->buttons[k_board[7][4]]->setText("♔");
    this->buttons[k_board[7][5]]->setText("♗");
    this->buttons[k_board[7][6]]->setText("♘");
    this->buttons[k_board[7][7]]->setText("♖");

    for (auto const &[square, piece] : k_pieces)
        this->buttons[square]->setEnabled(
            piece && piece->colour == k_current_player
        );
}

void MainWindow::selectPiece(Square const &square) {
    if (!this->selectedPiece) {
        this->prepareMove(square);

        return;
    }

    if (this->currentSquare != square)
        this->makeMove(square);

    this->currentSquare = {0, 0};
    this->selectedPiece = nullptr;

    for (auto const &[square, piece] : k_pieces)
        this->buttons[square]->setEnabled(
            piece && piece->colour == k_current_player
        );
}

void MainWindow::prepareMove(Square const square) {
    this->selectedPiece = k_pieces[square];

    if (!this->selectedPiece)
        return;

    this->currentSquare = square;

    std::set<Move> const &moves = this->selectedPiece->get_moves(square);

    for (QPushButton *button : this->buttons | std::views::values)
        button->setEnabled(false);

    for (auto const &[start, end] : moves)
        this->buttons[end]->setEnabled(true);

    this->buttons[square]->setEnabled(true);
}

void MainWindow::makeMove(Square square) {
    auto const [rank, file] = square;
    bool take = false;

    Square const &new_pos =
        k_board[this->currentSquare.rank][this->currentSquare.file];
    std::swap(k_pieces[square], k_pieces[new_pos]);

    QPushButton *nextButton = this->buttons[k_board[rank][file]];
    QPushButton *currentButton = this->buttons[new_pos];

    QString text = nextButton->text();

    if (!text.isEmpty()) {
        text = "";

        delete k_pieces[new_pos];
        k_pieces[new_pos] = nullptr;

        take = true;
    }

    nextButton->setText(currentButton->text());
    currentButton->setText(text);

    Colour const colour = this->selectedPiece->colour;

    if (k_current_player == Colour::white) {
        k_current_player = Colour::black;
        this->player->setText("Black to play");
    } else {
        k_current_player = Colour::white;
        this->player->setText("White to play");
    }

    bool const check =
        dynamic_cast<King *>(k_pieces[k_king_pos[k_current_player]])
            ->is_checked();

    for (Piece *piece : k_pieces | std::views::values)
        if (auto *pawn = dynamic_cast<Pawn *>(piece);
            pawn && pawn->colour == colour)
            pawn->can_be_en_passanted = false;

    bool promote = false, castle = false;

    if (auto *pawn = dynamic_cast<Pawn *>(this->selectedPiece); pawn) {
        pawn->moved = true;

        if (std::int32_t const rank_diff = rank - this->currentSquare.rank;
            rank_diff == -2 || rank_diff == 2) {
            pawn->can_be_en_passanted = true;
        } else if ((colour == Colour::white && rank == 0) ||
                   (colour == Colour::black && rank == 7)) {
            PromotionWindow promotion_window(
                colour, square, this->buttons[square], this->k_font_size, this
            );

            promotion_window.exec();

            promote = true;
        } else if ((this->currentSquare.file > 0 &&
                    file == this->currentSquare.file - 1) ||
                   (this->currentSquare.file < 7 &&
                    file == this->currentSquare.file - 1)) {
            auto *pawn = dynamic_cast<Pawn *>(
                k_pieces[k_board[this->currentSquare.rank][file]]
            );

            if (pawn && pawn->colour != this->selectedPiece->colour &&
                pawn->can_be_en_passanted) {
                delete k_pieces[k_board[this->currentSquare.rank][file]];
                k_pieces[k_board[this->currentSquare.rank][file]] = nullptr;

                this->buttons[k_board[this->currentSquare.rank][file]]->setText(
                    ""
                );
            }
        }
    } else if (auto *king = dynamic_cast<King *>(this->selectedPiece)) {
        k_king_pos[colour] = k_board[rank][file];

        king->moved = true;

        for (Piece *piece : k_pieces | std::views::values)
            if (auto *rook = dynamic_cast<Rook *>(piece);
                rook && rook->colour == colour)
                rook->can_castle = false;

        if (std::int32_t const file_diff = file - this->currentSquare.file;
            file_diff == -2 || file_diff == 2) {
            Rank const rank_ = colour == Colour::white ? 7 : 0;

            Square start, end;

            bool long_castle;

            if (file == 2) {
                start = k_board[rank_][0];
                end = k_board[rank][3];
                long_castle = true;
            } else {
                start = k_board[rank_][7];
                end = k_board[rank_][5];
                long_castle = false;
            }

            std::swap(k_pieces[start], k_pieces[end]);

            this->buttons[end]->setText(this->buttons[start]->text());
            this->buttons[start]->setText("");

            this->record->addCastle(colour, long_castle);

            castle = true;
        }
    } else if (auto *rook = dynamic_cast<Rook *>(this->selectedPiece)) {
        rook->can_castle = false;
    }

    bool flag = false;

    for (auto const &[square, piece] : k_pieces) {
        if (!piece || piece->colour != k_current_player ||
            piece->get_moves(square).empty())
            continue;

        flag = true;

        break;
    }

    bool checkmate = false;

    if (!flag) {
        if (check) {
            this->player->setText(
                std::format(
                    "Checkmate! {} wins!",
                    k_current_player == Colour::black ? "White" : "Black"
                )
                    .c_str()
            );

            checkmate = true;
        } else {
            this->player->setText("Stalemate");
        }

        for (QPushButton *button : this->buttons | std::views::values)
            button->setEnabled(false);
    }

    std::string t;

    if (Piece const *piece = k_pieces[square]; typeid(*piece) == typeid(Pawn))
        t = "";
    else if (typeid(*piece) == typeid(Knight))
        t = "N";
    else if (typeid(*piece) == typeid(Bishop))
        t = "B";
    else if (typeid(*piece) == typeid(Rook))
        t = "R";
    else if (typeid(*piece) == typeid(Queen))
        t = "Q";
    else if (typeid(*piece) == typeid(King))
        t = "K";

    if (promote)
        this->record->addPromotion(
            colour, {this->currentSquare, square}, t, check, checkmate, take
        );
    else if (!castle)
        this->record->addMove(
            colour, {this->currentSquare, square}, t, check, checkmate, take
        );
}

void MainWindow::newGame() {
    for (Piece *&piece : k_pieces | std::views::values) {
        delete piece;

        piece = nullptr;
    }

    for (QPushButton *button : this->buttons | std::views::values)
        button->setText("");

    k_king_pos[Colour::white] = k_board[7][4];
    k_king_pos[Colour::black] = k_board[0][4];

    k_current_player = Colour::white;

    this->currentSquare = {0, 0};
    this->selectedPiece = nullptr;

    this->record->clearRecords();

    this->setBoard();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    for (Piece const *piece : k_pieces | std::views::values)
        delete piece;

    QDialog::closeEvent(event);
}