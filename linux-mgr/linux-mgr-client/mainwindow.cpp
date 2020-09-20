#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Test.inl"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_dlgSet(this)
{
    ui->setupUi(this);

    // 全局性能数据列表
    ui->listWidget->addItem(QString("cpu"));
    ui->listWidget->addItem(QString("memory"));

    // 状态栏
    m_labelShowStatus   = new QLabel;
    ui->statusBar->addWidget(m_labelShowStatus, 1);

    ui->tableView->setSortingEnabled(true);

    connect(ui->actionSet, SIGNAL(triggered()), this, SLOT(OnTriggered()));
    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(OnItemClicked(QListWidgetItem*)));
    connect(&m_dlgSet,SIGNAL(updateSysinfo(QString)),this,SLOT(OnUpdateSysinfo(QString)));

    // 采样定时器
    startTimer(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnTriggered()
{
    m_dlgSet.show();
}

void MainWindow::OnUpdateSysinfo(QString qstrMsg)
{
    // 刷新系统信息
    qDebug()<<qstrMsg;
    m_objSysInfoParser.Exe(qstrMsg);
    this->updateSystemInfo();
}

void MainWindow::OnItemClicked(QListWidgetItem *item)
{
    if(item->text()=="cpu")
    {
        qDebug() << "cpu";
        if(m_widCpuStat==NULL)
        {
            m_widCpuStat = new WidCpuStat(this);
            ui->scrollArea_2->setWidget(m_widCpuStat);
            m_widCpuStat->SetCpuSeriesCount(3);
        }
    }
}

void MainWindow::updateSystemInfo()
{
    DRONE_CPU_INFO & cpuInfo = m_objSysInfoParser.m_struCpuInfo;
    ui->listWidget2->addItem(QString("cpu : %1 * %2")
                             .arg(cpuInfo.model_name)
                             .arg(cpuInfo.processor));
    ui->listWidget2->addItem(QString("cpu frequency : %1MHz")
                             .arg(cpuInfo.cpu_MHz));
    ui->listWidget2->addItem(QString("cpu arch : x%1")
                             .arg(cpuInfo.clflush_size));
}

void MainWindow::updateGlobalStat()
{

}

void MainWindow::updateProcList()
{
    QList<DRONE_PID_STAT> & qliProcStatList = m_topParser.m_objProcStatParser.m_qlProcList;
    QStandardItemModel* model = new QStandardItemModel(this);
    QStringList qlHeader;
    qlHeader<<"PID"<<"CMD"<<"State"<<"Cpu Rate";
    model->setHorizontalHeaderLabels(qlHeader);
    for(int i = 0; i<qliProcStatList.size();++i)
    {
        model->setItem(i, 0, new QStandardItem(QString("%1").arg(qliProcStatList[i].Pid)));
        model->setItem(i, 1, new QStandardItem(QString("%1").arg(qliProcStatList[i].Comm)));
        model->setItem(i, 2, new QStandardItem(QString("%1").arg(qliProcStatList[i].State)));
        int cpuRate = 0;
        if(m_topParser.m_objSataParser.m_qlSysRate.size()>0
                && m_topParser.m_objSataParser.m_qlSysRate[0].deltaTotalCpuTime!=0)
        {
            cpuRate = m_topParser.m_objProcStatParser.m_qlPidRate[i].deltaProcCpuTime
                            *100/m_topParser.m_objSataParser.m_qlSysRate[0].deltaTotalCpuTime;
        }
//        if(cpuRate>0)
//        {
//            printf("");
//        }
        model->setItem(i, 3, new QStandardItem(QString("%1%").arg(cpuRate)));
    }
    ui->tableView->setModel(model);
}

void MainWindow::timerEvent(QTimerEvent *t)
{
    (void)t;
    if(!GetGlobalDroneClient().IsSericeAvailable())
    {
        m_labelShowStatus->setText(QString("service offline"));
        return;
    }
    m_labelShowStatus->setText(QString("service online"));
    QString qstrRes;
    if(GetGlobalDroneClient().Proto_PROC_TOP(qstrRes)==-1)
    {
        m_labelShowStatus->setText(QString("service error"));
        return;
    }
    m_topParser.Exe(qstrRes);
//    qDebug()<< "timer";
    if(ui->listWidget->selectedItems().size()!=0)
    {
        static int x = 0;
        if(ui->listWidget->selectedItems()[0]->text()=="cpu")
        {//在可视的情况下刷新数据
            m_widCpuStat->UpdateCpuRate(x, m_topParser.m_objSataParser.m_qlSysRate);
            x++;
        }
    }
    if(ui->tableView->isVisible())
    {
        // 进程列表
        this->updateProcList();
    }
}
