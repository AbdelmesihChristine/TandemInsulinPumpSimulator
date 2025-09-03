#include "BolusDeliveryWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QLabel>
#include <QDoubleValidator>
#include <QButtonGroup>
#include "UserProfile.h"

BolusDeliveryWidget::BolusDeliveryWidget(UserProfileManager* profileMgr,
                                         PumpController* pumpCtrl,
                                         CgmSimulator* cgmSim,
                                         QWidget *parent)
    : QWidget(parent)
    , userProfileManager(profileMgr)
    , pumpController(pumpCtrl)
    , cgmSimulator(cgmSim)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // A row of radio buttons to pick BG source: manual vs. CGM
    QHBoxLayout* bgSourceLayout = new QHBoxLayout();
    useManualBgRadio = new QRadioButton("Manual BG");
    useCgmBgRadio    = new QRadioButton("CGM BG");
    bgSourceLayout->addWidget(useManualBgRadio);
    bgSourceLayout->addWidget(useCgmBgRadio);
    mainLayout->addLayout(bgSourceLayout);

    // Default: manual BG
    useManualBgRadio->setChecked(true);

    connect(useManualBgRadio, &QRadioButton::toggled, this, &BolusDeliveryWidget::onToggleBgSource);

    // BG input field
    QHBoxLayout* bgLayout = new QHBoxLayout();
    bgLayout->addWidget(new QLabel("BG (mmol/L):"));
    bgInput = new QLineEdit(this);
    bgInput->setValidator(new QDoubleValidator(0, 30, 1, this));
    bgLayout->addWidget(bgInput);
    mainLayout->addLayout(bgLayout);

    // If we have a CGM, connect its signal to auto-update BG
    if (cgmSimulator) {
        connect(cgmSimulator, &CgmSimulator::bgUpdated,
                this, &BolusDeliveryWidget::onCgmBgUpdated);
    }

    // Carbs input
    QHBoxLayout* carbsLayout = new QHBoxLayout();
    carbsLayout->addWidget(new QLabel("Carbs (g):"));
    carbsInput = new QLineEdit(this);
    carbsInput->setValidator(new QDoubleValidator(0, 1000, 1, this));
    carbsLayout->addWidget(carbsInput);
    mainLayout->addLayout(carbsLayout);

    // IOB input
    QHBoxLayout* iobLayout = new QHBoxLayout();
    iobLayout->addWidget(new QLabel("Insulin on Board (U):"));
    iobInput = new QLineEdit(this);
    iobInput->setValidator(new QDoubleValidator(0, 100, 1, this));
    iobLayout->addWidget(iobInput);
    mainLayout->addLayout(iobLayout);

    // Suggested bolus output (read-only)
    QHBoxLayout* bolusLayout = new QHBoxLayout();
    bolusLayout->addWidget(new QLabel("Suggested Bolus (U):"));
    suggestedBolus = new QLineEdit(this);
    suggestedBolus->setReadOnly(true);
    bolusLayout->addWidget(suggestedBolus);
    mainLayout->addLayout(bolusLayout);

    // Extended bolus toggle
    extendedCheck = new QCheckBox("Extended Bolus?", this);
    mainLayout->addWidget(extendedCheck);

    // Extended bolus fraction and duration
    QHBoxLayout* extendLayout = new QHBoxLayout();
    extendLayout->addWidget(new QLabel("Extended %:"));
    extendedPercent = new QSpinBox(this);
    extendedPercent->setRange(0,100);
    extendedPercent->setValue(40);
    extendLayout->addWidget(extendedPercent);

    extendLayout->addWidget(new QLabel("Duration (hrs):"));
    extendedHours = new QSpinBox(this);
    extendedHours->setRange(1, 24);
    extendedHours->setValue(3);
    extendLayout->addWidget(extendedHours);
    mainLayout->addLayout(extendLayout);

    // Buttons to Calculate and Deliver
    calcButton = new QPushButton("Calculate Bolus", this);
    deliverButton = new QPushButton("Deliver Bolus", this);
    mainLayout->addWidget(calcButton);
    mainLayout->addWidget(deliverButton);

    connect(calcButton, &QPushButton::clicked, this, &BolusDeliveryWidget::onCalculateBolus);
    connect(deliverButton, &QPushButton::clicked, this, &BolusDeliveryWidget::onDeliverBolus);

    setLayout(mainLayout);

    // Force an update of the BG input's readOnly state.
    onToggleBgSource();
}

/**
 * @brief Calculates a suggested bolus based on the user profile's carb ratio,
 * correction factor, and target BG, minus the IOB.
 */
double BolusDeliveryWidget::calculateSuggestedBolus(double bgVal, double carbsVal, double iobVal)
{
    UserProfile prof = userProfileManager->getActiveProfile();

    // Food bolus = Carbs / CarbRatio
    double foodBolus = 0.0;
    if (prof.carbRatio > 0.0) {
        foodBolus = carbsVal / prof.carbRatio;
    }

    // Correction bolus if BG is above target
    double correction = 0.0;
    if (bgVal > prof.targetGlucose && prof.correctionFactor > 0.0) {
        double diff = bgVal - prof.targetGlucose;
        correction = diff / prof.correctionFactor;
    }

    double totalBeforeIOB = foodBolus + correction;

    // Subtract IOB, but not below 0
    double finalBolus = totalBeforeIOB - iobVal;
    if (finalBolus < 0) {
        finalBolus = 0.0;
    }

    return finalBolus;
}

/**
 * @brief Called when user clicks "Calculate Bolus".
 * Gathers BG (manual or CGM), carbs, IOB, then updates suggestedBolus.
 */
void BolusDeliveryWidget::onCalculateBolus()
{
    bool okBG, okCarbs, okIOB;
    double carbsVal = carbsInput->text().toDouble(&okCarbs);
    double iobVal   = iobInput->text().toDouble(&okIOB);

    // Decide BG from manual or CGM
    double bgVal = 0.0;
    if (useManualBgRadio->isChecked()) {
        bgVal = bgInput->text().toDouble(&okBG);
        if (!okBG) {
            QMessageBox::warning(this, "Invalid BG", "Please enter a valid BG value.");
            return;
        }
    } else {
        bgVal = (cgmSimulator ? cgmSimulator->getCurrentBg() : 7.0);
        okBG = true;
    }

    if (!okCarbs || !okIOB) {
        QMessageBox::warning(this, "Invalid Input", "Please enter valid Carbs/IOB values.");
        return;
    }

    // Calculate and display the suggestion
    double suggestion = calculateSuggestedBolus(bgVal, carbsVal, iobVal);
    suggestedBolus->setText(QString::number(suggestion, 'f', 2));
}

/**
 * @brief Called when user clicks "Deliver Bolus".
 * Uses the suggested bolus and extended fraction if any.
 */
void BolusDeliveryWidget::onDeliverBolus()
{
    bool ok = false;
    double total = suggestedBolus->text().toDouble(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Error", "No suggested bolus to deliver.");
        return;
    }
    if (total <= 0) {
        QMessageBox::information(this, "Bolus", "Bolus is 0U. Nothing to deliver.");
        return;
    }

    double frac = 0.0;
    int hours = 0;
    if (extendedCheck->isChecked()) {
        frac  = extendedPercent->value() / 100.0;
        hours = extendedHours->value();
    }

    // Build a note string for the history log
    QString notes = QString("Manual Bolus. BG=%1, Carbs=%2, IOB=%3")
        .arg(bgInput->text())
        .arg(carbsInput->text())
        .arg(iobInput->text());

    // Request the bolus via PumpController
    bool success = pumpController->requestBolus(total, notes, frac, hours);
    if(!success) {
        QMessageBox::warning(this, "Safety Check Failed",
                             "Cannot deliver bolus due to pump safety constraints.");
        return;
    }

    // If successful, notify user
    QMessageBox::information(this, "Bolus Delivered",
        QString("Delivered: %1 U").arg(total));
}

/**
 * @brief If using CGM BG, auto-fill the bgInput whenever a new BG arrives.
 */
void BolusDeliveryWidget::onCgmBgUpdated(double newBg)
{
    if (useCgmBgRadio->isChecked()) {
        bgInput->setText(QString::number(newBg, 'f', 1));
    }
}

/**
 * @brief Toggles the BG input between manual (editable) or CGM (readOnly).
 */
void BolusDeliveryWidget::onToggleBgSource()
{
    bool manual = useManualBgRadio->isChecked();
    bgInput->setReadOnly(!manual);

    // If CGM is selected and we have a CGM reading, display it
    if (!manual && cgmSimulator) {
        double cgmVal = cgmSimulator->getCurrentBg();
        bgInput->setText(QString::number(cgmVal, 'f', 1));
    }
}
