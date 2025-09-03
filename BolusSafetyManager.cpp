#include "BolusSafetyManager.h"
#include <QTime>

BolusSafetyManager::BolusSafetyManager()
    : totalDailyBolus(0.0)
{
    // No special init
}

/**
 * @brief Checks if a new bolus is allowed given the single/daily/cooldown constraints.
 */
bool BolusSafetyManager::canDeliverBolus(double amount, QString &errorMessage)
{
    if (amount <= 0) {
        errorMessage = "Bolus must be > 0.";
        return false;
    }
    if (amount > maxSingleBolus) {
        errorMessage = QString("Exceeds max single bolus of %1U.").arg(maxSingleBolus);
        return false;
    }
    if (totalDailyBolus + amount > maxDailyBolus) {
        errorMessage = QString("Exceeds daily bolus limit of %1U.").arg(maxDailyBolus);
        return false;
    }

    if (lastBolusTime.isValid()) {
        int secsSinceLast = lastBolusTime.secsTo(QTime::currentTime());
        if (secsSinceLast < cooldownMinutes * 60) {
            int remain = (cooldownMinutes * 60) - secsSinceLast;
            int remainMin = remain / 60;
            errorMessage = QString("Wait %1 more minute(s) before another bolus.").arg(remainMin);
            return false;
        }
    }

    return true;
}

/**
 * @brief Records a delivered bolus by incrementing totalDailyBolus
 * and updating lastBolusTime.
 */
void BolusSafetyManager::recordBolus(double amount)
{
    totalDailyBolus += amount;
    lastBolusTime = QTime::currentTime();
}
