#include "widcpustat.h"
#include "ui_widcpustat.h"
#include <cassert>

WidCpuStat::WidCpuStat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidCpuStat)
{
    ui->setupUi(this);
}

WidCpuStat::~WidCpuStat()
{
    delete ui;
}

void WidCpuStat::UpdateCpuRate(int x, QList<DRONE_SYS_RATE> qlSysRate)
{
    if(qlSysRate.size()!=m_chart->series().size())
    {
        return;
    }
    if(qlSysRate.size()==0)
    {
        return;
    }
    for(int i=0;i<qlSysRate.size();++i)
    {
        QVector<QPointF> points=dynamic_cast<QLineSeries *>(m_chart->series()[i])->pointsVector();
        points.append(QPointF(x,qlSysRate[i].deltaProcCpuTime*100/qlSysRate[i].deltaTotalCpuTime));

        if(points.size()>m_max){//达到限值
            points.pop_front();
        }
        dynamic_cast<QLineSeries *>(m_chart->series()[i])->replace(points);

        if(x>60)
        {
            m_chart->axisX()->setRange(x-59,x);
        }
    }
}

void WidCpuStat::SetCpuSeriesCount(unsigned int uN)
{
    // QCharts配置
    m_chart = new QChart();
    for(unsigned int i=0;i<uN;++i)
    {
        QLineSeries *series = new QLineSeries();
        series->append(QPointF(0,0));
        series->setName("cpu" + (i==0?"":QString("%1").arg(i)));
        m_chart->addSeries(series);
    }
//    series->setPen(QPen(Qt::blue,1,Qt::SolidLine)); //设置画笔
    //series->setColor(QColor(0,0,255));  //设置颜色

    m_chart->createDefaultAxes();   //设置默认的坐标系，如果不设置程序是运行不了的
    m_chart->axisX()->setRange(0, 60);//设置图表坐标轴的范围，可以不设置，自动调节的
    m_chart->axisY()->setRange(0, 100);

    QChartView *chartView = new QChartView(m_chart);
    chartView->setRenderHint(QPainter::Antialiasing);  //抗锯齿渲染
    ui->horizontalLayout->addWidget(chartView);
}
