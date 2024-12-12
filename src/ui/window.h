#ifndef WINDOW_H
#define WINDOW_H

#include "../square.h"

#include <qapplication.h>
#include <qdialog.h>

class QLabel;

struct Piece;
class MoveRecord;

class MainWindow final : public QDialog {
  public:
    explicit MainWindow(QWidget *parent = nullptr);

  private:
    QLabel *player = nullptr;
    MoveRecord *record = nullptr;

    Piece *selectedPiece = nullptr;
    Square currentSquare;

    std::map<Square, QPushButton *> buttons;

    std::int32_t const k_font_size =
        QFontMetrics(QApplication::font()).horizontalAdvance(' ') * 16;

    void setBoard();

    void selectPiece(Square const &square);

    void prepareMove(Square square);

    void makeMove(Square square);

    void newGame();

    void closeEvent(QCloseEvent *event) override;
};

#endif // WINDOW_H
