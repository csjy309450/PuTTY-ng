#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dlgset.h"
#include "widcpustat.h"
#include "drone.h"

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void timerEvent(QTimerEvent *t);
    void updateSystemInfo();
    void updateGlobalStat();
    void updateProcList();

public slots:
//    void handleTimeout();
    void OnTriggered();
    void OnItemClicked(QListWidgetItem *item);
    void OnUpdateSysinfo(QString qstrMsg);

private:
    QTimer * m_Timer;
    WidCpuStat * m_widCpuStat=NULL;
    DlgSet m_dlgSet;
    QLabel * m_labelShowStatus;

    TopParser m_topParser;
    SysInfoParser m_objSysInfoParser;
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
