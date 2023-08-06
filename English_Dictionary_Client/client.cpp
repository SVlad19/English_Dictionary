#include "client.h"

Client::Client()
{
    socket = new QTcpSocket;
}

Client::~Client()
{
    if(socket){
         delete socket;
    }
}

bool Client::ConnectToHost(QString ip)
{
    socket->connectToHost(ip,48001);
    socket->waitForConnected();

    if(socket->state() == QAbstractSocket::ConnectedState){

        QMessageBox messageBox;
        messageBox.setWindowTitle("Connection");
        messageBox.setText("We did it!");
        messageBox.setIcon(QMessageBox::Information);

        messageBox.addButton(QMessageBox::Ok);
        messageBox.exec();


        return true;

    }
    return false;
}

QTcpSocket *Client::GetSocket()
{
    return socket;
}

void Client::SendToServer(QByteArray &data)
{
    if (socket->state() == QAbstractSocket::ConnectedState) {
            QDataStream out(socket);
            out.setVersion(QDataStream::Qt_6_3);
            out << data;
            qDebug() << "SendToServer::data = " << data;
            socket->waitForBytesWritten(3000);
            qDebug() << "Database has been sent";
        } else {
            qDebug() << "Don't have connection!";
        }
}

