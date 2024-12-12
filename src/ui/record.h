#ifndef RECORD_H
#define RECORD_H

#include "../colour.h"
#include "../move/move.h"
#include "../pieces/pieces.h"

#include <qtablewidget.h>

class MoveRecord final : public QTableWidget {
  public:
    explicit MoveRecord(QWidget *parent = nullptr);

    void addCastle(Colour colour, bool longCastle);

    void addPromotion(
        Colour colour, Move move, std::string result, bool checkmate,
        bool check, bool take
    );

    void addMove(
        Colour colour, Move move, std::string piece, bool check, bool checkmate,
        bool take
    );

    void clearRecords();

  private:
    void addMoveBase(Colour colour);
};

#endif // RECORD_H
