#include "AlertDialog.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include "HistoryRecord.h"

AlertDialog::AlertDialog(HistoryManager* historyMgr, QWidget *parent)
    : QDialog(parent)
    , historyManager(historyMgr)
{
    setWindowTitle("Pump Alerts");
    resize(400, 300);

    QVBoxLayout* layout = new QVBoxLayout(this);

    table = new QTableWidget(this);
    table->setColumnCount(2);
    table->setHorizontalHeaderLabels({"Time", "Message"});
    table->horizontalHeader()->setStretchLastSection(true);

    layout->addWidget(table);
    setLayout(layout);

    updateAlerts();
}

void AlertDialog::updateAlerts()
{
    const auto& records = historyManager->getRecords();

    // Collect only warnings from the general history
    QVector<HistoryRecord> warnings;
    for (const auto& rec : records) {
        if (rec.getRecordType() == RecordType::Warning) {
            warnings.append(rec);
        }
    }

    table->setRowCount(warnings.size());

    for (int i = 0; i < warnings.size(); ++i) {
        const auto& rec = warnings[i];
        table->setItem(i, 0, new QTableWidgetItem(rec.getTimestamp()));
        table->setItem(i, 1, new QTableWidgetItem(rec.getNotes()));
    }
}
