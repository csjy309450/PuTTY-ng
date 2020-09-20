#pragma once

#include <QtWidgets/QMainWindow>
#include "mainwindow.h"
#include "ui_linuxmgrclient.h"

class linuxmgrclient : public QMainWindow
{
    Q_OBJECT

public:
    linuxmgrclient(QWidget *parent = Q_NULLPTR);

private slots:
    //    void handleTimeout();
    void OnTriggered();
private:
    Ui::linuxmgrclientClass ui;
    MainWindow * process_monitor;
};
