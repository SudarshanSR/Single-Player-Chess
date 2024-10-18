#include "ui.h"

#include "../move/move.h"
#include "../pieces/pieces.h"
#include "../vars.h"

#include <qboxlayout.h>
#include <qgridlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <ranges>

namespace {
class PromotionWindow final : public QDialog {
  public:
    PromotionWindow(
        Colour colour, Square const &square, QPushButton *button, int font_size,
        QWidget *parent
    );

  private:
    Square const &square;
    Colour const colour;

    template <typename Piece>
        requires std::is_base_of_v<::Piece, Piece>
    void promote(QString const &text, QPushButton *button);
};

PromotionWindow::PromotionWindow(
    Colour const colour, Square const &square, QPushButton *button,
    int const font_size, QWidget *parent
)
    : QDialog(parent), square(square), colour(colour) {
    this->setWindowFlags(
        Qt::WindowType::Window | Qt::WindowType::WindowTitleHint |
        Qt::WindowType::CustomizeWindowHint
    );

    this->setAttribute(Qt::WidgetAttribute::WA_DeleteOnClose);

    QPushButton *queen, *rook, *bishop, *knight;

    if (colour == Colour::black) {
        queen = new QPushButton("♛", this);
        rook = new QPushButton("♜", this);
        bishop = new QPushButton("♝", this);
        knight = new QPushButton("♞", this);

        connect(queen, &QPushButton::clicked, [this, button]() -> void {
            this->promote<Queen>("♛", button);
        });
        connect(rook, &QPushButton::clicked, [this, button]() -> void {
            this->promote<Rook>("♜", button);
        });
        connect(bishop, &QPushButton::clicked, [this, button]() -> void {
            this->promote<Bishop>("♝", button);
        });
        connect(knight, &QPushButton::clicked, [this, button]() -> void {
            this->promote<Knight>("♞", button);
        });
    } else {
        queen = new QPushButton("♕", this);
        rook = new QPushButton("♖", this);
        bishop = new QPushButton("♗", this);
        knight = new QPushButton("♘", this);

        connect(queen, &QPushButton::clicked, [this, button]() -> void {
            this->promote<Queen>("♕", button);
        });
        connect(rook, &QPushButton::clicked, [this, button]() -> void {
            this->promote<Rook>("♖", button);
        });
        connect(bishop, &QPushButton::clicked, [this, button]() -> void {
            this->promote<Bishop>("♗", button);
        });
        connect(knight, &QPushButton::clicked, [this, button]() -> void {
            this->promote<Knight>("♘", button);
        });
    }

    auto *layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel("Promote to:"));

    for (QPushButton *button : std::array{queen, rook, bishop, knight}) {
        button->setMaximumSize(font_size, font_size);
        button->setMinimumSize(font_size, font_size);
        button->setStyleSheet("QPushButton { font : 32px; }");
        layout->addWidget(button);
    }
}

template <typename Piece>
    requires std::is_base_of_v<::Piece, Piece>
void PromotionWindow::promote(QString const &text, QPushButton *button) {
    delete k_pieces[this->square];

    k_pieces[this->square] = new Piece(this->colour);

    button->setText(text);

    this->close();
}
} // namespace

UI::UI(QWidget *parent) : QDialog(parent) {
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

    markings->addWidget(new QLabel("1"), 0, 1, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("2"), 0, 2, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("3"), 0, 3, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("4"), 0, 4, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("5"), 0, 5, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("6"), 0, 6, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("7"), 0, 7, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("8"), 0, 8, Qt::AlignmentFlag::AlignCenter);

    markings->addWidget(new QLabel("h"), 1, 0, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("g"), 2, 0, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("f"), 3, 0, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("e"), 4, 0, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("d"), 5, 0, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("c"), 6, 0, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("b"), 7, 0, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("a"), 8, 0, Qt::AlignmentFlag::AlignCenter);

    markings->addWidget(new QLabel("h"), 1, 9, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("g"), 2, 9, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("f"), 3, 9, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("e"), 4, 9, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("d"), 5, 9, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("c"), 6, 9, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("b"), 7, 9, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("a"), 8, 9, Qt::AlignmentFlag::AlignCenter);

    markings->addWidget(new QLabel("1"), 9, 1, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("2"), 9, 2, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("3"), 9, 3, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("4"), 9, 4, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("5"), 9, 5, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("6"), 9, 6, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("7"), 9, 7, Qt::AlignmentFlag::AlignCenter);
    markings->addWidget(new QLabel("8"), 9, 8, Qt::AlignmentFlag::AlignCenter);

    markings->addLayout(board, 1, 1, 8, 8);

    auto *newGame = new QPushButton("New Game", this);
    connect(newGame, &QPushButton::clicked, [this]() -> void {
        this->newGame();
    });

    auto *layout = new QVBoxLayout(this);
    layout->setSizeConstraint(QGridLayout::SizeConstraint::SetFixedSize);
    layout->addWidget(this->player, Qt::AlignmentFlag::AlignCenter);
    layout->addLayout(markings);
    layout->addWidget(newGame);
}

void UI::setBoard() {
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

void UI::selectPiece(Square const &square) {
    auto const &[rank, file] = square;

    if (!this->selectedPiece) {
        this->selectedPiece = k_pieces[square];

        if (!this->selectedPiece)
            return;

        this->currentRank = rank;
        this->currentFile = file;

        std::set<Move> const &moves = this->selectedPiece->get_moves(square);

        for (QPushButton *button : this->buttons | std::views::values)
            button->setEnabled(false);

        for (auto const &[start, end] : moves)
            this->buttons[end]->setEnabled(true);

        this->buttons[square]->setEnabled(true);

        return;
    }

    if (this->currentRank != rank || this->currentFile != file) {
        Square const &new_pos = k_board[this->currentRank][this->currentFile];
        std::swap(k_pieces[square], k_pieces[new_pos]);

        QPushButton *nextButton = this->buttons[k_board[rank][file]];
        QPushButton *currentButton = this->buttons[new_pos];

        QString text = nextButton->text();

        if (!text.isEmpty()) {
            text = "";

            delete k_pieces[new_pos];
            k_pieces[new_pos] = nullptr;
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

        for (Piece *piece : k_pieces | std::views::values)
            if (auto *pawn = dynamic_cast<Pawn *>(piece);
                pawn && pawn->colour == colour)
                pawn->can_be_en_passanted = false;

        if (auto *pawn = dynamic_cast<Pawn *>(this->selectedPiece); pawn) {
            pawn->moved = true;

            if (std::int32_t const rank_diff = rank - this->currentRank;
                rank_diff == -2 || rank_diff == 2) {
                pawn->can_be_en_passanted = true;
            } else if ((colour == Colour::white && rank == 0) ||
                       (colour == Colour::black && rank == 7)) {
                PromotionWindow promotion_window(
                    colour, square, this->buttons[square], this->k_font_size,
                    this
                );

                promotion_window.exec();
            } else if ((this->currentFile > 0 && file == this->currentFile - 1
                       ) ||
                       (this->currentFile < 7 && file == this->currentFile - 1
                       )) {
                auto *pawn = dynamic_cast<Pawn *>(
                    k_pieces[k_board[this->currentRank][file]]
                );

                if (pawn && pawn->colour != this->selectedPiece->colour &&
                    pawn->can_be_en_passanted) {
                    delete k_pieces[k_board[this->currentRank][file]];
                    k_pieces[k_board[this->currentRank][file]] = nullptr;

                    this->buttons[k_board[this->currentRank][file]]->setText(
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

            if (std::int32_t const file_diff = file - this->currentFile;
                file_diff == -2 || file_diff == 2) {
                Rank const rank_ = colour == Colour::white ? 7 : 0;

                Square start, end;

                if (file == 2) {
                    start = k_board[rank_][0];
                    end = k_board[rank][3];
                } else {
                    start = k_board[rank_][7];
                    end = k_board[rank_][5];
                }

                std::swap(k_pieces[start], k_pieces[end]);

                this->buttons[end]->setText(this->buttons[start]->text());
                this->buttons[start]->setText("");
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

        if (!flag) {
            if (dynamic_cast<King *>(k_pieces[k_king_pos[k_current_player]])
                    ->is_checked())
                this->player->setText(
                    std::format(
                        "Checkmate! {} wins!",
                        k_current_player == Colour::black ? "White" : "Black"
                    )
                        .c_str()
                );

            else
                this->player->setText("Stalemate");

            for (QPushButton *button : this->buttons | std::views::values)
                button->setEnabled(false);

            return;
        }
    }

    this->currentRank = 0;
    this->currentFile = 0;
    this->selectedPiece = nullptr;

    for (auto const &[square, piece] : k_pieces)
        this->buttons[square]->setEnabled(
            piece && piece->colour == k_current_player
        );
}

void UI::newGame() {
    for (Piece *&piece : k_pieces | std::views::values) {
        delete piece;

        piece = nullptr;
    }

    for (QPushButton *button : this->buttons | std::views::values)
        button->setText("");

    k_king_pos[Colour::white] = k_board[7][4];
    k_king_pos[Colour::black] = k_board[0][4];

    k_current_player = Colour::white;

    this->currentRank = 0;
    this->currentFile = 0;
    this->selectedPiece = nullptr;

    this->setBoard();
}

void UI::closeEvent(QCloseEvent *event) {
    for (Piece const *piece : k_pieces | std::views::values)
        delete piece;

    QDialog::closeEvent(event);
}