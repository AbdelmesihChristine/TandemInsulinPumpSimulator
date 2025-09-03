#ifndef BOLUSRECORD_H
#define BOLUSRECORD_H

#include <QString>

class BolusRecord
{
public:
    BolusRecord(double amount, const QString& notes)
        : amount(amount), notes(notes) {}

    double getAmount() const { return amount; }
    QString getNotes() const { return notes; }

private:
    double amount;
    QString notes;
};

#endif // BOLUSRECORD_H
