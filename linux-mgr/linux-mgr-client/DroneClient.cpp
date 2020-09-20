#include "DroneClient.h"

#include <QApplication>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QtTest/qtest.h>

const int TIMEOUT = (5 * 1000);

DroneClient g_droneClient;

DroneClient & GetGlobalDroneClient()
{
    return g_droneClient;
}

DroneClient::DroneClient()
    : m_bAvailable(false)
{

}

bool DroneClient::IsSericeAvailable()
{
    return m_bAvailable;
}

void DroneClient::CheckSericeAvailable()
{
    QString qstrMsg;
    GetGlobalDroneClient().Proto_HELLO(qstrMsg);
    if(qstrMsg.indexOf("/hello")==-1)
    {
        m_bAvailable = false;
    }
    else{
        m_bAvailable = true;
    }
}

void DroneClient::SetSericeAddr(QString qstrIP, unsigned int uiPort)
{
    m_qstrIP=(qstrIP);
    m_uiPort=(uiPort);
}

int DroneClient::Proto_SYSINFO(QString & res)
{
    return HttpGet("/drone/sysinfo", res);
}

int DroneClient::Proto_HELLO(QString & res)
{
    return HttpGet("/drone/hello", res);
}

int DroneClient::Proto_PROC_TOP(QString & res)
{
    return HttpGet("/drone/proc/top", res);
}

int DroneClient::HttpGet(const QString &qstrUri, QString & qstrRes, void* pConf)
{
    (void)pConf;
    QNetworkAccessManager m_HttpMgr;
    //设置url
    QNetworkRequest requestInfo;
    QString qstrUrl = "http://" + m_qstrIP+":"+QString("%1").arg(m_uiPort)+qstrUri;
    requestInfo.setUrl(QUrl(qstrUrl));

    //添加事件循环机制，返回后再运行后面的
    QEventLoop eventLoop;
    QNetworkReply *reply =  m_HttpMgr.get(requestInfo);
    QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();       //block until finish
    //错误处理
    if (reply->error() == QNetworkReply::NoError)
    {
        qDebug() << "request protobufHttp NoError";
    }
    else
    {
        qDebug()<<"request protobufHttp handle errors here";
        QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        //statusCodeV是HTTP服务器的相应码，reply->error()是Qt定义的错误码，可以参考QT的文档
        qDebug( "request protobufHttp found error ....code: %d %d\n", statusCodeV.toInt(), (int)reply->error());
        qDebug(qPrintable(reply->errorString()));
        return -1;
    }
    //请求返回的结果
    qstrRes.clear();
    qstrRes = reply->readAll();
    qDebug() << qstrRes.mid(0, 100);
    return 0;
}

int DroneClient::HttpPost(const QString &qstrUri, const QString &qstrBody, QString & qstrRes, void* pConf)
{
//    (void)pConf;
//    QString _result;
//    QNetworkRequest _request;
//    _request.setUrl(QUrl(qstrUri));
////    QSslConfiguration _sslCon = _request.sslConfiguration();
////    _sslCon.setPeerVerifyMode(QSslSocket::VerifyNone);
////    _request.setSslConfiguration(_sslCon);
//    _request.setHeader(QNetworkRequest::ContentTypeHeader,
//                       QString("application/x-www-form-urlencoded"));

//    QNetworkReply *_reply = m_NtwkAccManager.post(_request, qstrBody.toLatin1());
//    _reply->ignoreSslErrors();


//    QTime _t;
//    _t.start();

//    bool _timeout = false;

//    while (!_reply->isFinished()) {
//        QApplication::processEvents();
//        if (_t.elapsed() >= TIMEOUT) {
//            _timeout = true;
//            break;
//        }
//    }

//    if (!_timeout && _reply->error() == QNetworkReply::NoError) {
//        _result = _reply->readAll();
//    }

//    _reply->deleteLater();

    return 0;
}
