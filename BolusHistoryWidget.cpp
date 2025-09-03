#include "BolusHistoryWidget.h"
#include "BolusHistoryManager.h"
#include "BolusRecord.h"

#include <QVBoxLayout>
#include <QHeaderView>
#include <QTableWidgetItem>

BolusHistoryWidget::BolusHistoryWidget(BolusHistoryManager* manager, QWidget* parent)
    : QWidget(parent)
    , historyManager(manager)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    table = new QTableWidget(this);
    table->setColumnCount(2);
    table->setHorizontalHeaderLabels({"Amount (U)", "Notes"});
    table->horizontalHeader()->setStretchLastSection(true);
    layout->addWidget(table);

    setLayout(layout);

    // Populate the table
    updateTable();
}

void BolusHistoryWidget::updateTable()
{
    const auto& records = historyManager->getRecords();
    table->setRowCount(static_cast<int>(records.size()));

    for (int i = 0; i < (int)records.size(); ++i) {
        const BolusRecord& rec = records[i];
        table->setItem(i, 0, new QTableWidgetItem(QString::number(rec.getAmount())));
        table->setItem(i, 1, new QTableWidgetItem(rec.getNotes()));
    }
}
