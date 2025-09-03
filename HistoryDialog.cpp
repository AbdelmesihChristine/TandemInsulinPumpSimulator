#include "HistoryDialog.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include "HistoryRecord.h"

HistoryDialog::HistoryDialog(HistoryManager* manager, QWidget* parent)
    : QDialog(parent)
    , historyManager(manager)
{
    setWindowTitle("History Log");
    resize(500, 300);

    QVBoxLayout* layout = new QVBoxLayout(this);

    // We show 4 columns: Time, Type, Amount, and Notes
    table = new QTableWidget(this);
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"Time", "Type", "Amount (U)", "Notes"});
    table->horizontalHeader()->setStretchLastSection(true);

    layout->addWidget(table);
    setLayout(layout);

    updateTable();
}

/**
 * @brief updateTable scans the entire history and displays each record
 * in a new row.
 */
void HistoryDialog::updateTable()
{
    const auto& records = historyManager->getRecords();
    table->setRowCount(static_cast<int>(records.size()));

    for (int i = 0; i < (int)records.size(); ++i) {
        const HistoryRecord& rec = records[i];

        // Time
        table->setItem(i, 0, new QTableWidgetItem(rec.getTimestamp()));

        // Type
        QString typeStr;
        switch(rec.getRecordType()) {
        case RecordType::ManualBolus: typeStr = "Manual Bolus"; break;
        case RecordType::AutoBolus:   typeStr = "Auto Bolus";   break;
        case RecordType::CgmReading:  typeStr = "CGM Reading";  break;
        case RecordType::Warning:     typeStr = "Warning";      break;
        default:                      typeStr = "Other";        break;
        }
        table->setItem(i, 1, new QTableWidgetItem(typeStr));

        // Amount (only relevant for boluses)
        if (rec.getInsulinAmount() > 0) {
            table->setItem(i, 2,
                new QTableWidgetItem(QString::number(rec.getInsulinAmount(), 'f', 2)));
        } else {
            table->setItem(i, 2, new QTableWidgetItem("-"));
        }

        // Notes
        table->setItem(i, 3, new QTableWidgetItem(rec.getNotes()));
    }
}
