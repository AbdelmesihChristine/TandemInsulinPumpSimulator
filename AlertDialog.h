#ifndef ALERTDIALOG_H
#define ALERTDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include "HistoryManager.h"

/**
 * @brief AlertDialog displays a table of recent Warnings (low battery,
 * low insulin, or critical BG). It filters the general HistoryManager log
 * for only entries of type RecordType::Warning.
 */
class AlertDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AlertDialog(HistoryManager* historyMgr, QWidget *parent = nullptr);

    /**
     * @brief Pulls the latest Warnings from the history and updates the table.
     */
    void updateAlerts();

private:
    HistoryManager* historyManager;
    QTableWidget* table;
};

#endif // ALERTDIALOG_H
