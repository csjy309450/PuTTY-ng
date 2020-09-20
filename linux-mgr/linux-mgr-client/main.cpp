#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include "drone.h"
#include "linuxmgrclient.h"
//#include "Test.inl"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    linuxmgrclient w;
    w.show();

//    DroneClient client(&a);
//    QString qstrMsg = client.httpGet("http://192.168.1.105:5432/drone/proc/top");
//    qDebug()<<"[info] " << qstrMsg.mid(0,10);
//    qDebug() << "tip4";

    return a.exec();
}
