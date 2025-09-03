#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDateTime>
#include <QIcon>
#include <QFrame>

/**
 * @brief Constructs the MainWindow, creates all managers/controllers,
 * initializes the UI, and starts the CGM + warning checks.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Create all the backend managers and controllers
    userProfileManager  = new UserProfileManager(this);
    bolusSafetyManager  = new BolusSafetyManager();
    historyManager      = new HistoryManager();
    cgmSimulator        = new CgmSimulator(this);
    pumpController      = new PumpController(
                              userProfileManager,
                              historyManager,
                              bolusSafetyManager,
                              cgmSimulator,
                              this);

    // Create a WarningChecker to track battery/insulin usage and BG
    warningChecker = new WarningChecker(historyManager, cgmSimulator, this);

    // Example: set reservoir and battery to low values to show warnings:
    warningChecker->setInsulinLevel(4.0);  // e.g. only 4 units left
    warningChecker->setBatteryLevel(8);    // e.g. 8% battery

    // Build the battery indicator UI
    batteryBarsWidget = new QWidget(this);
    QHBoxLayout* batteryBarsLayout = new QHBoxLayout(batteryBarsWidget);
    batteryBarsLayout->setSpacing(2);
    for(int i=0; i<5; i++){
        QFrame* bar = new QFrame(this);
        bar->setFixedSize(8, 20);
        bar->setStyleSheet("background-color: #00ff00;"); // green bars
        batteryBarsLayout->addWidget(bar);
    }
    batteryTextLabel = new QLabel("100%", this);

    // Build the insulin reservoir UI
    insulinBarsWidget = new QWidget(this);
    QHBoxLayout* insulinBarsLayout = new QHBoxLayout(insulinBarsWidget);
    insulinBarsLayout->setSpacing(2);
    for(int i=0; i<5; i++){
        QFrame* bar = new QFrame(this);
        bar->setFixedSize(8, 20);
        bar->setStyleSheet("background-color: #3399ff;"); // blue bars
        insulinBarsLayout->addWidget(bar);
    }
    insulinTextLabel = new QLabel("300U", this);

    // Time display label
    timeLabel = new QLabel("--:--", this);
    timeLabel->setAlignment(Qt::AlignCenter);

    // Create navigation buttons
    bolusButton   = new QPushButton(QIcon(":/icons/icons/drop.png"), "Bolus", this);
    profileButton = new QPushButton(QIcon(":/icons/icons/setting.png"), "Profiles", this);
    historyButton = new QPushButton(QIcon(":/icons/icons/history.png"), "History", this);
    alertButton   = new QPushButton(QIcon(":/icons/icons/warning.png"), "Alerts", this);

    // CGM Graph area
    cgmGraphWidget = new CGMGraphWidget(cgmSimulator, this);

    // Connect button signals to the appropriate slots
    connect(bolusButton,   &QPushButton::clicked, this, &MainWindow::openBolusDialog);
    connect(profileButton, &QPushButton::clicked, this, &MainWindow::openProfiles);
    connect(historyButton, &QPushButton::clicked, this, &MainWindow::openHistory);
    connect(alertButton,   &QPushButton::clicked, this, &MainWindow::openAlerts);

    // Create the dialogs
    bolusDialog   = new BolusDialog(userProfileManager, pumpController, cgmSimulator, this);
    profileDialog = new ProfileDialog(userProfileManager, this);
    histDialog    = new HistoryDialog(historyManager, this);
    alertDialog   = new AlertDialog(historyManager, this);

    // Layout the top bar with battery, time, and insulin indicators
    QWidget* central = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(central);

    QHBoxLayout* topLayout = new QHBoxLayout();
    QVBoxLayout* batteryLayout = new QVBoxLayout();
    batteryLayout->addWidget(batteryBarsWidget);
    batteryLayout->addWidget(batteryTextLabel);

    QVBoxLayout* insulinLayout = new QVBoxLayout();
    insulinLayout->addWidget(insulinBarsWidget);
    insulinLayout->addWidget(insulinTextLabel);

    topLayout->addLayout(batteryLayout);
    topLayout->addStretch();
    topLayout->addWidget(timeLabel);
    topLayout->addStretch();
    topLayout->addLayout(insulinLayout);

    // Next row: navigation buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(bolusButton);
    buttonLayout->addWidget(profileButton);
    buttonLayout->addWidget(historyButton);
    buttonLayout->addWidget(alertButton);

    // Add top and button layouts, plus the CGM graph
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(cgmGraphWidget);

    central->setLayout(mainLayout);
    setCentralWidget(central);

    // Set the window title
    setWindowTitle("t:slim X2 Pump Simulation");

    // Start the CGM simulation and the warning checker
    cgmSimulator->start();
    warningChecker->startMonitoring();

    // Dark background styling
    setStyleSheet(R"(
        QWidget {
            background-color: #1c1b1b;
            color: #eeeeee;
        }
    )");

    // Timer that updates clock/battery display every second
    uiRefreshTimer = new QTimer(this);
    connect(uiRefreshTimer, &QTimer::timeout, this, &MainWindow::updateTime);
    uiRefreshTimer->start(1000);

    updateTime();
}

/**
 * @brief MainWindow destructor.
 * Qt automatically cleans up child widgets and dynamically allocated
 * objects with a valid parent, so we have minimal cleanup here.
 */
MainWindow::~MainWindow()
{
}

/**
 * @brief Opens the Manual Bolus dialog (BolusDialog).
 */
void MainWindow::openBolusDialog()
{
    bolusDialog->exec();
}

/**
 * @brief Opens the Profile management dialog.
 */
void MainWindow::openProfiles()
{
    profileDialog->exec();
}

/**
 * @brief Opens the History dialog, which shows logs of all events.
 */
void MainWindow::openHistory()
{
    histDialog->updateTable();
    histDialog->exec();
}

/**
 * @brief Opens the Alerts dialog, showing any warnings (battery low, insulin low, etc.).
 */
void MainWindow::openAlerts()
{
    alertDialog->updateAlerts();
    alertDialog->exec();
}

/**
 * @brief Called every second by uiRefreshTimer.
 * Updates the displayed time, battery %, and insulin reservoir if needed.
 */
void MainWindow::updateTime()
{
    // Show actual local date/time (though the pump sim has a "simulated time" for BG events).
    QDateTime now = QDateTime::currentDateTime();
    timeLabel->setText(now.toString("hh:mm AP\nddd, dd MMM"));

    // Display battery level from WarningChecker
    int battery = warningChecker->getBatteryLevel();
    batteryTextLabel->setText(QString("%1%").arg(battery));

    // Display insulin reservoir from WarningChecker
    double ins = warningChecker->getInsulinLevel();
    insulinTextLabel->setText(QString("%1U").arg(ins,0,'f',0));
}
