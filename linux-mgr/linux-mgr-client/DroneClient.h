#ifndef DRONECLIENT_H
#define DRONECLIENT_H

#include <QApplication>
#include <QNetworkAccessManager>
#include <QString>

/**
 * @brief The DroneClient class \n
 * recv data from Drone Server
 */
class DroneClient
{
public:
    DroneClient();

    void SetSericeAddr(QString qstrIP, unsigned int uiPort);
    bool IsSericeAvailable();
    void CheckSericeAvailable();

    int Proto_HELLO(QString & res);
    int Proto_SYSINFO(QString & res);
    int Proto_PROC_TOP(QString & res);
private:
    int HttpPost(const QString &qstrUri, const QString &qstrBody, QString & qstrRes, void* pConf=NULL);
    int HttpGet(const QString &qstrUri, QString & qstrRes, void* pConf=NULL);
private:
    bool m_bAvailable;
    QString m_qstrIP;
    unsigned int m_uiPort;
};

DroneClient & GetGlobalDroneClient();

#endif // DRONECLIENT_H
