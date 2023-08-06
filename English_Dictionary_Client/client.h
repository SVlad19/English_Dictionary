#include <QStandardItemModel>
#include <QMessageBox>
#include <QTcpSocket>

class Client : public QObject
{
    Q_OBJECT
public:
    Client();
    ~Client();

    bool ConnectToHost(QString);
    QTcpSocket* GetSocket();
    void SendToServer(QByteArray& data);

public slots:

private:
    QTcpSocket* socket;

};


