#include "CgmSimulator.h"
#include <QRandomGenerator>
#include <QtMath>

CgmSimulator::CgmSimulator(QObject* parent)
    : QObject(parent)
    , currentBg(7.0)  // Starting BG around 7 mmol/L
    , totalSimMinutes(0)
{
    connect(&updateTimer, &QTimer::timeout, this, &CgmSimulator::onTimerTick);
}

double CgmSimulator::getCurrentBg() const
{
    return currentBg;
}

/**
 * @brief start begins the 1-second timer that represents 5 simulated minutes each tick.
 */
void CgmSimulator::start()
{
    updateTimer.start(1000);
}

/**
 * @brief getSimTimeStr returns "HH:MM" as the simulated time.
 */
QString CgmSimulator::getSimTimeStr() const
{
    int hours = totalSimMinutes / 60;
    int mins  = totalSimMinutes % 60;
    return QString("%1:%2")
            .arg(hours, 2, 10, QLatin1Char('0'))
            .arg(mins, 2, 10, QLatin1Char('0'));
}

std::deque<double> CgmSimulator::getLastSixReadings() const
{
    return lastSix;
}

/**
 * @brief onTimerTick is called each real second. We treat it as 5 simulated minutes,
 * randomly change the BG, then emit bgUpdated(newBg).
 */
void CgmSimulator::onTimerTick()
{
    // Each second => 5 sim minutes
    totalSimMinutes += 5;

    // Random walk in BG: +/- up to 0.2
    double delta = (QRandomGenerator::global()->bounded(20) - 10) / 50.0;
    currentBg += delta;

    // Clamp BG to a safe range
    if (currentBg < 2.5)  currentBg = 2.5;
    if (currentBg > 18.0) currentBg = 18.0;

    pushReading(currentBg);

    // Notify observers (PumpController, CGMGraphWidget, etc.)
    emit bgUpdated(currentBg);
}

/**
 * @brief pushReading inserts a new reading into the rolling queue,
 * ensuring we only keep the last 6.
 */
void CgmSimulator::pushReading(double bg)
{
    lastSix.push_back(bg);
    if (lastSix.size() > 6) {
        lastSix.pop_front();
    }
}
