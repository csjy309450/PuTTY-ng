#ifndef WIDCPUSTAT_H
#define WIDCPUSTAT_H

#include <QList>
#include <QWidget>
#include <QtCharts>
#include <QtCharts/QLineSeries>
#include <QChartView>
#include "drone.h"

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class WidCpuStat;
}

class WidCpuStat : public QWidget
{
    Q_OBJECT

public:
    explicit WidCpuStat(QWidget *parent = 0);
    ~WidCpuStat();

    void SetCpuSeriesCount(unsigned int uN);
    void UpdateCpuRate(int x, QList<DRONE_SYS_RATE> qlSysRate);

private:
    Ui::WidCpuStat *ui;
    QChart * m_chart;
    QValueAxis * m_axisX;
    QValueAxis * m_axisY;
    QList<QLineSeries *> m_qlSeries;

    const int m_max = 100;
};

#endif // WIDCPUSTAT_H
