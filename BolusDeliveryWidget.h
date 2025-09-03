#ifndef BOLUSDELIVERYWIDGET_H
#define BOLUSDELIVERYWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QSpinBox>
#include <QRadioButton>
#include "PumpController.h"
#include "UserProfileManager.h"
#include "CgmSimulator.h"

/**
 * @brief BolusDeliveryWidget is the UI for manually delivering a bolus.
 * The user can enter Carbs, BG, and IOB, and choose immediate vs. extended.
 * They can also pick Manual BG or auto-populate from CGM.
 */
class BolusDeliveryWidget : public QWidget {
    Q_OBJECT

public:
    BolusDeliveryWidget(UserProfileManager* profileMgr,
                        PumpController* pumpCtrl,
                        CgmSimulator* cgmSim = nullptr,
                        QWidget *parent = nullptr);

private slots:
    void onCalculateBolus();
    void onDeliverBolus();
    void onCgmBgUpdated(double newBg);
    void onToggleBgSource();

private:
    double calculateSuggestedBolus(double bgVal, double carbsVal, double iobVal);

    UserProfileManager* userProfileManager;
    PumpController*     pumpController;
    CgmSimulator*       cgmSimulator;

    // Radio buttons to choose Manual BG or CGM BG
    QRadioButton* useManualBgRadio;
    QRadioButton* useCgmBgRadio;

    // Input fields
    QLineEdit* bgInput;
    QLineEdit* carbsInput;
    QLineEdit* iobInput;
    QLineEdit* suggestedBolus;

    // Extended bolus options
    QCheckBox* extendedCheck;
    QSpinBox*  extendedPercent;
    QSpinBox*  extendedHours;

    // Action buttons
    QPushButton* calcButton;
    QPushButton* deliverButton;
};

#endif // BOLUSDELIVERYWIDGET_H
