#include "WarningChecker.h"
#include "HistoryRecord.h"
#include <QMessageBox>

WarningChecker::WarningChecker(HistoryManager* hist, CgmSimulator* cgm, QObject* parent)
    : QObject(parent),
      history(hist),
      cgmSimulator(cgm),
      batteryLevel(100),
      insulinReservoir(200.0)
{
    // The timer calls onCheck() every 30s.
    connect(&checkTimer, &QTimer::timeout, this, &WarningChecker::onCheck);
}

void WarningChecker::startMonitoring()
{
    // Check every 30 seconds (real time)
    checkTimer.start(30000);
}

void WarningChecker::stopMonitoring()
{
    checkTimer.stop();
}

/**
 * @brief onCheck is called every 30s.
 * Depletes battery by 1% for demonstration, checks thresholds for battery/insulin/BG.
 */
void WarningChecker::onCheck()
{
    // For demonstration, degrade battery by 1%
    if (batteryLevel > 0) {
        batteryLevel -= 1;
    }

    // Battery warnings
    if (batteryLevel == 5) {
        logWarning("Battery critically low!");
    } else if (batteryLevel == 20) {
        logWarning("Battery low!");
    }

    // Insulin warnings
    // We only reduce insulin if the pump delivers a bolus
    // (see PumpController).
    if (insulinReservoir <= 5) {
        logWarning("Insulin critically low!");
    } else if (insulinReservoir <= 20) {
        logWarning("Insulin low!");
    }

    // BG warnings (critically low <3.9 or high >13.9)
    if (cgmSimulator) {
        double bg = cgmSimulator->getCurrentBg();
        if (bg < 3.9) {
            logWarning(QString("BG critically low (%.1f)!" ).arg(bg));
        } else if (bg > 13.9) {
            logWarning(QString("BG critically high (%.1f)!" ).arg(bg));
        }
    }
}

/**
 * @brief logWarning writes a record to HistoryManager and shows a dark-themed QMessageBox.
 */
void WarningChecker::logWarning(const QString& msg)
{
    if (!history || !cgmSimulator) return;

    // Log the event
    history->addRecord({
        cgmSimulator->getSimTimeStr(),
        RecordType::Warning,
        0.0,
        msg
    });

    // Show a pop-up
    showDarkWarning("Pump Warning", msg);
}

/**
 * @brief showDarkWarning uses a custom stylesheet to display a dark-themed warning box.
 */
void WarningChecker::showDarkWarning(const QString& title, const QString& text)
{
    QMessageBox box(QMessageBox::Warning, title, text, QMessageBox::Ok);
    box.setStyleSheet(R"(
        QMessageBox {
            background-color: #2a2a2a;
        }
        QLabel {
            color: white;
            font-weight: bold;
        }
        QPushButton {
            background-color: #444;
            color: white;
            border: 1px solid #666;
            padding: 6px;
            border-radius: 4px;
        }
        QPushButton:hover {
            background-color: #555;
        }
    )");
    box.exec();
}
