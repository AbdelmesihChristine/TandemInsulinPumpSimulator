#ifndef BOLUSSAFETYMANAGER_H
#define BOLUSSAFETYMANAGER_H

#include <QString>
#include <QTime>

/**
 * @brief BolusSafetyManager checks constraints:
 * - maximum single bolus
 * - total daily bolus limit
 * - a cooldown time between boluses
 */
class BolusSafetyManager
{
public:
    BolusSafetyManager();

    /**
     * @param amount (insulin units)
     * @param errorMessage (out param to show reason if false)
     * @return true if safe to deliver, false otherwise
     */
    bool canDeliverBolus(double amount, QString &errorMessage);

    /**
     * @brief recordBolus increments the daily total and updates the lastBolusTime
     */
    void recordBolus(double amount);

private:
    double maxSingleBolus = 10.0;
    double maxDailyBolus  = 30.0; // e.g. 30U daily limit
    int cooldownMinutes   = 10;   // must wait 10min between boluses

    double totalDailyBolus;
    QTime lastBolusTime;
};

#endif // BOLUSSAFETYMANAGER_H
