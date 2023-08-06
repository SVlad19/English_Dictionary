#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myserver.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tvDataBase->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("./EnglishDictionary.db");

    if(database.open()){
        qDebug()<<"database is open";
    }else{
        qDebug()<<"database isn`t open";
    }

    query = new QSqlQuery(database);
    query->exec("CREATE TABLE EnglishWords(Word TEXT, Transcript TEXT)");
    query->exec("DELETE FROM EnglishWords");

    model = new QSqlTableModel(this,database);
    model->setTable("EnglishWords");

    model->select();

    ui->tvDataBase->setModel(model);

    server = std::make_unique<MyServer>();
    connect(server.get(),&MyServer::info,this,&MainWindow::FillDatabase);
}

MainWindow::~MainWindow()
{
    delete query;
    delete model;
    delete ui;
}

void MainWindow::FillDatabase()
{
    model->removeRows(0,model->rowCount());

    QByteArray data = server->GetDatabase();
    QList<QByteArray> str = data.split('\n');

    for (const QByteArray& line : str) {
        if (line.isEmpty()){
            continue;
        }

        QList<QByteArray> parts = line.split(':');
        QSqlRecord temp = model->record();

        temp.setValue(0, QString::fromUtf8(parts[0]));
        temp.setValue(1, QString::fromUtf8(parts[1]));

        model->insertRecord(model->rowCount(), temp);
    }

}


void MainWindow::on_btnSend_clicked()
{
    QSqlRecord temp = model->record();

    temp.setValue(0,ui->leWord->text());
    temp.setValue(1,ui->leTranscription->text());

    model->insertRecord(model->rowCount(), temp);

    QString str = ui->leWord->text() + ' ' +ui->leTranscription->text();

    ui->leWord->clear();
    ui->leTranscription->clear();

    server->SendToClient(str);
}


