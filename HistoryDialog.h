#ifndef HISTORYDIALOG_H
#define HISTORYDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include "HistoryManager.h"

/**
 * @brief HistoryDialog shows a table of all events in the HistoryManager,
 * including boluses, CGM readings, warnings, etc.
 */
class HistoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HistoryDialog(HistoryManager* manager, QWidget* parent = nullptr);
    void updateTable();

private:
    HistoryManager* historyManager;
    QTableWidget* table;
};

#endif // HISTORYDIALOG_H
