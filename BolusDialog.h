#ifndef BOLUSDIALOG_H
#define BOLUSDIALOG_H

#include <QDialog>
#include "BolusDeliveryWidget.h"

/**
 * @brief BolusDialog is a simple QDialog container that holds
 * a BolusDeliveryWidget for manual bolus entry.
 */
class BolusDialog : public QDialog
{
    Q_OBJECT
public:
    explicit BolusDialog(UserProfileManager* profileMgr,
                         PumpController* pumpCtrl,
                         CgmSimulator* sim,
                         QWidget *parent = nullptr);

private:
    BolusDeliveryWidget* bolusWidget;
};

#endif // BOLUSDIALOG_H
