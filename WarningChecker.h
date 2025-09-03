#ifndef WARNINGCHECKER_H
#define WARNINGCHECKER_H

#include <QObject>
#include <QTimer>
#include "HistoryManager.h"
#include "CgmSimulator.h"

/**
 * @brief WarningChecker periodically checks battery level, insulin reservoir,
 * and BG to produce warnings (RecordType::Warning). It logs these
 * and shows a message box to alert the user.
 */
class WarningChecker : public QObject
{
    Q_OBJECT
public:
    explicit WarningChecker(HistoryManager* hist, CgmSimulator* cgm, QObject* parent=nullptr);

    void setBatteryLevel(int level) { batteryLevel = level; }
    int  getBatteryLevel() const { return batteryLevel; }

    void setInsulinLevel(double units) { insulinReservoir = units; }
    double getInsulinLevel() const { return insulinReservoir; }

    /**
     * @brief Optionally displays a styled warning message box.
     */
    void showDarkWarning(const QString& title, const QString& text);

public slots:
    /**
     * @brief Start/stop the internal timer that checks every 30s.
     */
    void startMonitoring();
    void stopMonitoring();

private slots:
    void onCheck();

private:
    HistoryManager* history;
    CgmSimulator*   cgmSimulator;
    QTimer checkTimer;

    int batteryLevel;         // 0..100%
    double insulinReservoir;  // in units

    void logWarning(const QString& msg);
};

#endif // WARNINGCHECKER_H
