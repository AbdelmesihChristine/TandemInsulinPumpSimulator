#include "CGMGraphWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

CGMGraphWidget::CGMGraphWidget(CgmSimulator* simulator, QWidget *parent)
    : QWidget(parent)
    , cgmSimulator(simulator)
    , timeCounter(0)
{
    // Create a chart, line series, and axes to plot BG over time
    chart = new QChart();
    series = new QLineSeries();
    axisX = new QValueAxis();
    axisY = new QValueAxis();

    axisX->setTitleText("Time (sec)");
    axisY->setTitleText("BG (mmol/L)");
    axisX->setRange(0, 60);
    axisY->setRange(2, 16);

    chart->addSeries(series);
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    // Set a dark background
    chart->setBackgroundBrush(QBrush(QColor("#1c1b1b")));
    axisX->setGridLineColor(QColor("#555555"));
    axisY->setGridLineColor(QColor("#555555"));
    axisX->setLabelsBrush(QBrush(Qt::white));
    axisY->setLabelsBrush(QBrush(Qt::white));
    axisX->setTitleBrush(QBrush(Qt::white));
    axisY->setTitleBrush(QBrush(Qt::white));
    series->setColor(QColor("#00ccff"));

    // Put the chart in a QChartView
    chartView = new QChartView(chart, this);
    chartView->setRenderHint(QPainter::Antialiasing);

    // ComboBox to pick 1h, 3h, or 6h range (12s, 36s, 72s real time)
    rangeComboBox = new QComboBox(this);
    rangeComboBox->addItem("1h (12s)", 12);
    rangeComboBox->addItem("3h (36s)", 36);
    rangeComboBox->addItem("6h (72s)", 72);

    connect(rangeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CGMGraphWidget::onRangeChanged);

    // Layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(new QLabel("BG Graph:", this));
    mainLayout->addWidget(rangeComboBox);
    mainLayout->addWidget(chartView);
    setLayout(mainLayout);

    // If there's a simulator, connect its bgUpdated signal
    if (cgmSimulator) {
        connect(cgmSimulator, &CgmSimulator::bgUpdated, this, &CGMGraphWidget::updateGraph);
    }
}

/**
 * @brief updateGraph is called whenever a new BG reading arrives.
 * We increment timeCounter by 1 each second, then plot that BG reading.
 */
void CGMGraphWidget::updateGraph(double bgValue)
{
    timeCounter++;
    series->append(timeCounter, bgValue);

    // Auto-scroll if timeCounter exceeds the current axis max
    if (timeCounter > axisX->max()) {
        double rangeSize = axisX->max() - axisX->min();
        axisX->setRange(timeCounter - rangeSize, timeCounter);
    }
}

/**
 * @brief onRangeChanged adjusts the X-axis range based on the user's selection
 * (1h=12s, 3h=36s, 6h=72s).
 */
void CGMGraphWidget::onRangeChanged(int index)
{
    int maxSec = rangeComboBox->currentData().toInt();
    axisX->setRange(0, maxSec);
}
