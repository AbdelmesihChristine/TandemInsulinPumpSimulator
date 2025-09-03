#ifndef PUMPCONTROLLER_H
#define PUMPCONTROLLER_H

#include <QObject>
#include "UserProfileManager.h"
#include "HistoryManager.h"
#include "BolusSafetyManager.h"
#include "CgmSimulator.h"

/**
 * @brief PumpController ties together the CGM data, safety checks,
 * history logging, and user inputs (manual bolus). It also
 * implements simple Control-IQ logic for auto basal or auto bolus.
 */
class PumpController : public QObject
{
    Q_OBJECT
public:
    explicit PumpController(UserProfileManager* profileMgr,
                            HistoryManager* histMgr,
                            BolusSafetyManager* safetyMgr,
                            CgmSimulator* cgmSim,
                            QObject* parent = nullptr);

    /**
     * @brief requestBolus attempts to deliver a manual bolus
     * (possibly extended), checking safety constraints
     * and logging events in the history.
     */
    bool requestBolus(double totalBolus,
                      const QString& notes,
                      double extendedFrac = 0.0,
                      int durationHrs = 0);

public slots:
    /**
     * @brief onCgmUpdated is triggered whenever a new BG reading arrives,
     * logs it, then runs the ControlIQ logic.
     */
    void onCgmUpdated(double newBg);

private:
    UserProfileManager* userProfileManager;
    HistoryManager*     historyManager;
    BolusSafetyManager* safetyManager;
    CgmSimulator*       cgmSimulator;

    /**
     * @brief runControlIQ attempts to predict BG 30min ahead and
     * adjust insulin delivery if needed (suspend, auto-correct).
     */
    void runControlIQ(double currentBg);

    /**
     * @brief deliverAutoBolus attempts an automatic correction bolus.
     */
    void deliverAutoBolus(double units, const QString& reason);
};

#endif // PUMPCONTROLLER_H
