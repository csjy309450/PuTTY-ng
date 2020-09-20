#include <qlogging.h>
#include "linuxmgrclient.h"

linuxmgrclient::linuxmgrclient(QWidget *parent)
    : QMainWindow(parent)
    , process_monitor(NULL)
{
    ui.setupUi(this);
    connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(OnTriggered()));
}

void linuxmgrclient::OnTriggered()
{
    if (!process_monitor)
    {
        process_monitor = new MainWindow(this);
    }
    process_monitor->show();
    //qDebug() << "linuxmgrclient::OnTriggered()";
}