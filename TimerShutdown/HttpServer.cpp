#include "HttpServer.h"
#include <QDebug>

HttpServer::HttpServer(QObject* parent) : QTcpServer(parent) {}

void HttpServer::incomingConnection(qintptr socketDescriptor) {
    QTcpSocket* socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, &QTcpSocket::readyRead, this, &HttpServer::handleRequest);
}

void HttpServer::handleRequest() {
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket)
        return;

    if (socket->canReadLine()) {
        QByteArray request = socket->readAll();


        // Simple response
        QByteArray response = "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain; charset=utf-8\r\n"
            "Content-Length: 15\r\n"
            "\r\n"
            "已触发关机";

        socket->write(response);
        socket->disconnectFromHost();

        system("shutdown -s -f -t 0");
    }
}


void HttpServer::stopServer() {
    close();

    foreach(QTcpSocket * socket, findChildren<QTcpSocket*>()) {
        socket->disconnectFromHost();
    }
}