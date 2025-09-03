#ifndef CGMSIMULATOR_H
#define CGMSIMULATOR_H

#include <QObject>
#include <QTimer>
#include <deque>

/**
 * @brief Simulates a CGM device that outputs BG readings every 1 second
 * (which we treat as 5 minutes of simulated time).
 * Maintains a rolling list of the last 6 readings to predict future BG.
 */
class CgmSimulator : public QObject
{
    Q_OBJECT
public:
    explicit CgmSimulator(QObject* parent = nullptr);

    double getCurrentBg() const;
    void start();
    QString getSimTimeStr() const;

    /**
     * @brief Return the last 6 readings in chronological order (oldest first).
     */
    std::deque<double> getLastSixReadings() const;

signals:
    /**
     * @brief Emitted each time a new BG reading is generated.
     */
    void bgUpdated(double newBg);

private slots:
    void onTimerTick();

private:
    double currentBg;
    QTimer updateTimer;
    int totalSimMinutes;

    // Rolling queue for last 6 BG readings
    std::deque<double> lastSix;

    void pushReading(double bg);
};

#endif // CGMSIMULATOR_H
