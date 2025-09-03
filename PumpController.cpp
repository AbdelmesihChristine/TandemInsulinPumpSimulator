#include "PumpController.h"
#include "HistoryRecord.h"
#include <QDebug>

PumpController::PumpController(UserProfileManager* profileMgr,
                               HistoryManager* histMgr,
                               BolusSafetyManager* safetyMgr,
                               CgmSimulator* cgmSim,
                               QObject* parent)
    : QObject(parent),
      userProfileManager(profileMgr),
      historyManager(histMgr),
      safetyManager(safetyMgr),
      cgmSimulator(cgmSim)
{
    // Whenever CGM updates a reading, we do onCgmUpdated
    connect(cgmSimulator, &CgmSimulator::bgUpdated,
            this, &PumpController::onCgmUpdated);
}

/**
 * @brief requestBolus handles a manual bolus request (carbs/correction).
 */
bool PumpController::requestBolus(double totalBolus,
                                  const QString& notes,
                                  double extendedFrac,
                                  int durationHrs)
{
    QString errorMsg;
    // First, check safety constraints
    if (!safetyManager->canDeliverBolus(totalBolus, errorMsg)) {
        return false;
    }

    // If extendedFrac is between 0 and 1, we split. Otherwise, all is immediate
    double immediate = totalBolus;
    double extended  = 0.0;
    if (extendedFrac > 0.0 && extendedFrac < 1.0) {
        immediate = totalBolus * (1.0 - extendedFrac);
        extended  = totalBolus * extendedFrac;
    }

    // Deliver immediate portion
    safetyManager->recordBolus(immediate);
    historyManager->addRecord({
        cgmSimulator->getSimTimeStr(),
        RecordType::ManualBolus,
        immediate,
        notes + " (Immediate portion)"
    });

    // Extended portion is delivered all at once for simplicity here
    if (extended > 0.0) {
        safetyManager->recordBolus(extended);
        historyManager->addRecord({
            cgmSimulator->getSimTimeStr(),
            RecordType::ManualBolus,
            extended,
            QString("Extended portion over %1hr").arg(durationHrs)
        });
    }

    return true;
}

/**
 * @brief onCgmUpdated logs the reading and runs the ControlIQ algorithm.
 */
void PumpController::onCgmUpdated(double newBg)
{
    // Log the CGM reading
    historyManager->addRecord({
        cgmSimulator->getSimTimeStr(),
        RecordType::CgmReading,
        0.0,
        QString("BG= %1 mmol/L").arg(newBg, 0, 'f', 1)
    });

    // Then run Control IQ logic
    runControlIQ(newBg);
}

/**
 * @brief runControlIQ: a simple approach to predict BG in 30 min
 * by looking at the difference between the oldest and newest
 * of the last 6 readings. If predicted <3.9, suspend. If >=14,
 * do auto correction. If >=10, increase basal, etc.
 */
void PumpController::runControlIQ(double currentBg)
{
    auto lastSix = cgmSimulator->getLastSixReadings();
    if (lastSix.size() < 6) {
        // Not enough data for a 30min trend
        return;
    }
    double first = lastSix.front();   // reading from ~30min ago
    double predicted = currentBg + (currentBg - first);

    // If predicted < 3.9 => suspend basal
    if (predicted < 3.9) {
        historyManager->addRecord({
            cgmSimulator->getSimTimeStr(),
            RecordType::Other,
            0.0,
            QString("Basal suspended by Control-IQ (predBG= %1)").arg(predicted,0,'f',1)
        });
        return;
    }

    // If predicted >=14 => deliver an auto-correction of 1U (example)
    if (predicted >= 14.0) {
        deliverAutoBolus(1.0, QString("Auto correction (predBG= %1)").arg(predicted,0,'f',1));
        return;
    }

    // If predicted >=10 => "increase basal"
    if (predicted >= 10.0) {
        historyManager->addRecord({
            cgmSimulator->getSimTimeStr(),
            RecordType::Other,
            0.0,
            QString("Basal increased by Control-IQ (predBG= %1)").arg(predicted,0,'f',1)
        });
    }
    // else do nothing
}

/**
 * @brief deliverAutoBolus tries an automatic bolus and logs it.
 * If safety check fails, logs a warning.
 */
void PumpController::deliverAutoBolus(double units, const QString& reason)
{
    QString errorMsg;
    if (!safetyManager->canDeliverBolus(units, errorMsg)) {
        // If we can't deliver it, log a warning
        historyManager->addRecord({
            cgmSimulator->getSimTimeStr(),
            RecordType::Warning,
            0.0,
            QString("Auto-bolus blocked: %1").arg(errorMsg)
        });
        return;
    }
    // Otherwise deliver it
    safetyManager->recordBolus(units);
    historyManager->addRecord({
        cgmSimulator->getSimTimeStr(),
        RecordType::AutoBolus,
        units,
        reason
    });
}
