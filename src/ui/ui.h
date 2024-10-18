#ifndef UI_H
#define UI_H

#include "../square.h"

#include <qapplication.h>
#include <qdialog.h>

class QLabel;

struct Piece;

class UI final : public QDialog {
  public:
    explicit UI(QWidget *parent = nullptr);

  private:
    QLabel *player = nullptr;

    Piece *selectedPiece = nullptr;
    Rank currentRank = 0;
    File currentFile = 0;

    std::map<Square, QPushButton *> buttons;

    std::int32_t const k_font_size =
        QFontMetrics(QApplication::font()).horizontalAdvance(' ') * 16;

    void setBoard();

    void selectPiece(Square const &square);

    void newGame();

    void closeEvent(QCloseEvent *event) override;
};

#endif // UI_H
