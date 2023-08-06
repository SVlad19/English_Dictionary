#include "myserver.h"

MyServer::MyServer()
{
    if(!listen(QHostAddress::AnyIPv4,48001)){
        qDebug()<<"Server couldn`t start!";
    }else{
        QMessageBox messageBox;
        messageBox.setWindowTitle("Server");
        messageBox.setText("Server created!");
        messageBox.setIcon(QMessageBox::Information);

        messageBox.addButton(QMessageBox::Ok);
        messageBox.exec();

        qDebug()<<"Server started!";
    }

}

MyServer::~MyServer()
{
    if(socket){
        delete socket;
    }
}

void MyServer::SendToClient(QString &message)
{
    if(socket->state() == QAbstractSocket::ConnectedState){
        data.clear();
        QDataStream out(&data,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Version::Qt_6_3);
        out << message;

        socket->write(data);
    }else{
        qDebug()<<"Don`t have connection!";
    }
}

QByteArray &MyServer::GetDatabase()
{
    return dataDatabase;
}

void MyServer::incomingConnection(qintptr socketDescriptor)
{
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket,&QTcpSocket::readyRead,this,&MyServer::Read);
    connect(socket,&QTcpSocket::disconnected,this,&MyServer::deleteLater);


    QMessageBox messageBox;
    messageBox.setWindowTitle("Connection");
    messageBox.setText("New connection");
    messageBox.setIcon(QMessageBox::Information);

    messageBox.addButton(QMessageBox::Ok);
    messageBox.exec();

    qDebug()<<"Client connected!"<<socket->socketDescriptor();
}

void MyServer::Read()
{
    dataDatabase.clear();

    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_3);
    if (in.status() == QDataStream::Ok) {
        qDebug() << "Read..";
        in >> dataDatabase;
        qDebug() << dataDatabase;
        emit info();
    } else {
        qDebug() << "DataStream error!";
    }
}


