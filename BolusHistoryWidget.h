#ifndef BOLUSHISTORYWIDGET_H
#define BOLUSHISTORYWIDGET_H

#include <QWidget>
#include <QTableWidget>

class BolusHistoryManager;

class BolusHistoryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BolusHistoryWidget(BolusHistoryManager* manager, QWidget* parent = nullptr);

private:
    void updateTable();

    BolusHistoryManager* historyManager;
    QTableWidget* table;
};

#endif // BOLUSHISTORYWIDGET_H
