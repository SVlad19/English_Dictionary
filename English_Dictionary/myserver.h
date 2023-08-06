#include <QObject>
#include <QDebug>
#include <QMessageBox>
#include <QTcpServer>
#include <QTcpSocket>

class MyServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit MyServer();
    ~MyServer();
    void SendToClient(QString& message);
    QByteArray& GetDatabase();

signals:
    void info();

public slots:
    void incomingConnection(qintptr socketDescriptor)override;
    void Read();

private:
    QTcpSocket* socket;
    QByteArray data;
    QByteArray dataDatabase;
};

