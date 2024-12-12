#include "record.h"

#include <qheaderview.h>

MoveRecord::MoveRecord(QWidget *parent) : QTableWidget(parent) {
    this->setColumnCount(2);
    this->setHorizontalHeaderLabels({"White", "Black"});
    this->horizontalHeader()->setSectionResizeMode(
        QHeaderView::ResizeMode::Stretch
    );
    this->setEditTriggers(NoEditTriggers);
}

void MoveRecord::addCastle(Colour const colour, bool const longCastle) {
    this->addMoveBase(colour);

    std::int32_t const column = colour == Colour::white ? 0 : 1;

    this->setItem(
        this->rowCount() - 1, column,
        new QTableWidgetItem(longCastle ? "O-O-O" : "O-O")
    );
}

void MoveRecord::addPromotion(
    Colour const colour, Move const move, std::string result,
    bool const checkmate, bool const check, bool const take
) {
    this->addMoveBase(colour);

    std::int32_t const column = colour == Colour::white ? 0 : 1;

    QString text;

    if (take) {
        if (checkmate)
            text = QString::fromStdString(
                std::format(
                    "{}x{}={}#", static_cast<std::string>(move.start),
                    static_cast<std::string>(move.end), result
                )
            );
        else if (check)
            text = QString::fromStdString(
                std::format(
                    "{}x{}={}+", static_cast<std::string>(move.start),
                    static_cast<std::string>(move.end), result
                )
            );
        else
            text = QString::fromStdString(
                std::format(
                    "{}x{}={}", static_cast<std::string>(move.start),
                    static_cast<std::string>(move.end), result
                )
            );
    } else {
        if (checkmate)
            text = QString::fromStdString(
                std::format(
                    "{}{}={}#", static_cast<std::string>(move.start),
                    static_cast<std::string>(move.end), result
                )
            );
        else if (check)
            text = QString::fromStdString(
                std::format(
                    "{}{}={}+", static_cast<std::string>(move.start),
                    static_cast<std::string>(move.end), result
                )
            );
        else
            text = QString::fromStdString(
                std::format(
                    "{}{}={}", static_cast<std::string>(move.start),
                    static_cast<std::string>(move.end), result
                )
            );
    }

    this->setItem(this->rowCount() - 1, column, new QTableWidgetItem(text));
}

void MoveRecord::addMove(
    Colour const colour, Move const move, std::string piece, bool const check,
    bool const checkmate, bool const take
) {
    this->addMoveBase(colour);

    std::int32_t const column = colour == Colour::white ? 0 : 1;

    QString text;

    if (take) {
        if (checkmate)
            text = QString::fromStdString(
                std::format(
                    "{}{}x{}#", piece, static_cast<std::string>(move.start),
                    static_cast<std::string>(move.end)
                )
            );
        else if (check)
            text = QString::fromStdString(
                std::format(
                    "{}{}x{}+", piece, static_cast<std::string>(move.start),
                    static_cast<std::string>(move.end)
                )
            );
        else
            text = QString::fromStdString(
                std::format(
                    "{}{}x{}", piece, static_cast<std::string>(move.start),
                    static_cast<std::string>(move.end)
                )
            );
    } else {
        if (checkmate)
            text = QString::fromStdString(
                std::format(
                    "{}{}{}#", piece, static_cast<std::string>(move.start),
                    static_cast<std::string>(move.end)
                )
            );
        else if (check)
            text = QString::fromStdString(
                std::format(
                    "{}{}{}+", piece, static_cast<std::string>(move.start),
                    static_cast<std::string>(move.end)
                )
            );
        else
            text = QString::fromStdString(
                std::format(
                    "{}{}{}", piece, static_cast<std::string>(move.start),
                    static_cast<std::string>(move.end)
                )
            );
    }

    this->setItem(this->rowCount() - 1, column, new QTableWidgetItem(text));
}

void MoveRecord::clearRecords() { this->setRowCount(0); }

void MoveRecord::addMoveBase(Colour const colour) {
    if (colour == Colour::black)
        return;

    this->insertRow(this->rowCount());
}