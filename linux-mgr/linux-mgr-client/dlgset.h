#ifndef DLGSET_H
#define DLGSET_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class DlgSet;
}

class DlgSet : public QDialog
{
    Q_OBJECT

public:
    explicit DlgSet(QWidget *parent = 0);
    ~DlgSet();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

signals:
    void updateSysinfo(QString qstrMsg);//定义子窗口向主窗口发送数据的信号

private:
    Ui::DlgSet *ui;
};

#endif // DLGSET_H
