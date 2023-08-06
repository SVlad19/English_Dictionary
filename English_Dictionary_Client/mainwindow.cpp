#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "client.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tbDataBase->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("./EnglishDictionary.db");
    if(database.open()){
        qDebug()<<"database is open";
    }else{
        qDebug()<<"database isn`t open";
    }

    query = new QSqlQuery(database);
    query->exec("CREATE TABLE EnglishWords(Word TEXT, Transcript TEXT)");

    model = new QSqlTableModel(this,database);
    model->setTable("EnglishWords");
    model->select();

    ui->tbDataBase->setModel(model);

    GetDataFormDatabase();

    pClient = std::make_unique<Client>();
}

MainWindow::~MainWindow()
{
    delete query;
    delete model;
    delete ui;
}

void MainWindow::GetDataFormDatabase()
{
    if(query->exec("SELECT Word, Transcript FROM EnglishWords")){
        qDebug()<<"Select ok!";
        QBuffer buffer(&data);
        buffer.open(QIODevice::WriteOnly);

        while(query->next()){
            QString word = query->value("Word").toString();
            QString transcription = query->value("Transcript").toString();

            QString datastring = QString("%1:%2\n").arg(word,transcription);

            qDebug()<<"Our datastring = "<<datastring;

            buffer.write(datastring.toUtf8());
        }
        buffer.close();

    }else{
        qDebug()<<"Select didn`t work!";
    }
}

void MainWindow::FillInfo()
{
    QDataStream in(pClient->GetSocket());
    in.setVersion(QDataStream::Version::Qt_6_3);
    if(in.status() == QDataStream::Ok){
        QString str;
        in >> str;

        QSqlRecord temp = model->record();
        QStringList parts = str.split(" ");

        temp.setValue(0,parts[0]);
        temp.setValue(1,parts[1]);

        model->insertRecord(model->rowCount(),temp);

    }else{
        qDebug()<<"DataStream error!";
    }
}


void MainWindow::on_btnConnect_clicked()
{
    if(pClient->ConnectToHost(ui->leIP->text())){
        ui->leIP->clear();
        qDebug()<<"Connected!";
        pClient->SendToServer(data);
        connect(pClient->GetSocket(),&QTcpSocket::readyRead,this,&MainWindow::FillInfo);
        connect(pClient->GetSocket(),&QTcpSocket::disconnected,pClient->GetSocket(),&QTcpSocket::deleteLater);
    }else{
        qDebug()<<"Failed";
    }

}


void MainWindow::on_btnRemove_clicked()
{
     pClient->SendToServer(data);
     model->removeRow(arrow);
}


void MainWindow::on_tbDataBase_clicked(const QModelIndex &index)
{
    arrow = index.row();
}

