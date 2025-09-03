#ifndef CGMGRAPHWIDGET_H
#define CGMGRAPHWIDGET_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QComboBox>
#include "CgmSimulator.h"

QT_CHARTS_USE_NAMESPACE

/**
 * @brief CGMGraphWidget displays a real-time line graph of BG readings
 * from the simulator, with adjustable time scale (1h, 3h, 6h in simulated terms).
 */
class CGMGraphWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CGMGraphWidget(CgmSimulator* simulator, QWidget *parent = nullptr);

private slots:
    void updateGraph(double bgValue);
    void onRangeChanged(int index);

private:
    CgmSimulator* cgmSimulator;
    QChartView* chartView;
    QLineSeries* series;
    QChart* chart;
    QValueAxis* axisX;
    QValueAxis* axisY;
    QComboBox*  rangeComboBox;

    int timeCounter; // increments by 1 each real second
};

#endif // CGMGRAPHWIDGET_H
