#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QObject>

class HttpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit HttpServer(QObject* parent = nullptr);
    void stopServer();  // 添加停止服务器的方法
protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void handleRequest();
};

#endif // HTTPSERVER_H
