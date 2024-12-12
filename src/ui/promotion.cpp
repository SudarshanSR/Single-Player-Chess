#include "promotion.h"

#include "../vars.h"

#include <qboxlayout.h>
#include <qdialog.h>
#include <qlabel.h>
#include <qpushbutton.h>

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

    for (QPushButton *pieceButton : std::array{queen, rook, bishop, knight}) {
        pieceButton->setMaximumSize(font_size, font_size);
        pieceButton->setMinimumSize(font_size, font_size);
        pieceButton->setStyleSheet("QPushButton { font : 32px; }");
        layout->addWidget(pieceButton);
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