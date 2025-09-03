#ifndef HISTORYRECORD_H
#define HISTORYRECORD_H

#include <QString>

/**
 * @brief RecordType categorizes events in the pump's history log
 * (manual bolus, auto bolus, CGM reading, warning, etc.).
 */
enum class RecordType {
    ManualBolus,
    AutoBolus,
    CgmReading,
    Warning,
    Other
};

/**
 * @brief HistoryRecord stores an event (timestamp, record type,
 * insulin amount if relevant, and notes).
 */
class HistoryRecord
{
public:
    HistoryRecord(const QString& time,
                  RecordType type,
                  double amount,
                  const QString& notes)
        : timestamp(time),
          recordType(type),
          insulinAmount(amount),
          recordNotes(notes)
    {}

    QString getTimestamp() const { return timestamp; }
    RecordType getRecordType() const { return recordType; }
    double getInsulinAmount() const { return insulinAmount; }
    QString getNotes() const { return recordNotes; }

private:
    QString timestamp;
    RecordType recordType;
    double insulinAmount;
    QString recordNotes;
};

#endif
