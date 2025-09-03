#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include "UserProfileManager.h"
#include "BolusSafetyManager.h"
#include "HistoryManager.h"
#include "PumpController.h"
#include "CgmSimulator.h"
#include "CGMGraphWidget.h"
#include "BolusDialog.h"
#include "ProfileDialog.h"
#include "HistoryDialog.h"
#include "AlertDialog.h"
#include "WarningChecker.h"

/**
 * @brief MainWindow is the top-level container for our insulin pump simulation UI.
 * It sets up the battery/insulin indicators, time display, navigation buttons,
 * and the CGM graph area. It also manages the major backend objects
 * (UserProfileManager, PumpController, etc.) and starts the CGM/Warning logic.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openBolusDialog();
    void openProfiles();
    void openHistory();
    void openAlerts();
    void updateTime();

private:
    // Core logic objects
    UserProfileManager* userProfileManager;
    BolusSafetyManager* bolusSafetyManager;
    HistoryManager*     historyManager;
    PumpController*     pumpController;
    CgmSimulator*       cgmSimulator;
    WarningChecker*     warningChecker;

    // UI elements for battery & insulin display
    QLabel* batteryTextLabel;
    QWidget* batteryBarsWidget;
    QLabel* insulinTextLabel;
    QWidget* insulinBarsWidget;
    QLabel* timeLabel;

    // Navigation buttons
    QPushButton* bolusButton;
    QPushButton* profileButton;
    QPushButton* historyButton;
    QPushButton* alertButton;

    // CGM Graph
    CGMGraphWidget* cgmGraphWidget;

    // Child Dialogs
    BolusDialog*   bolusDialog;
    ProfileDialog* profileDialog;
    HistoryDialog* histDialog;
    AlertDialog*   alertDialog;

    // Timer to periodically update UI time/battery display
    QTimer* uiRefreshTimer;
};

#endif // MAINWINDOW_H
