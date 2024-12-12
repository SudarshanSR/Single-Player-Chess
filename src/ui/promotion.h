#ifndef PROMOTION_H
#define PROMOTION_H

#include "../colour.h"
#include "../pieces/pieces.h"
#include "../square.h"

#include <qdialog.h>

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

#endif // PROMOTION_H
