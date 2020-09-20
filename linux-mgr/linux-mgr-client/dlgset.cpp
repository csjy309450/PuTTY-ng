#include "dlgset.h"
#include "ui_dlgset.h"

#include "DroneClient.h"

DlgSet::DlgSet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgSet)
{
    ui->setupUi(this);
}

DlgSet::~DlgSet()
{
    delete ui;
}

void DlgSet::on_buttonBox_clicked(QAbstractButton *button)
{
    (void)button;
    QString qstrWork = ui->lineEdit->text();
    unsigned int uiPort = ui->lineEdit_2->text().toUInt();
    if(qstrWork.size()==0)
    {
        return;
    }
    GetGlobalDroneClient().SetSericeAddr(qstrWork, uiPort);
    GetGlobalDroneClient().CheckSericeAvailable();
    if(GetGlobalDroneClient().IsSericeAvailable())
    {
        qDebug()<<"connect remote service";
        GetGlobalDroneClient().Proto_SYSINFO(qstrWork);
        emit this->updateSysinfo(qstrWork);
    }
}
