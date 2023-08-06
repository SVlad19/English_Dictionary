#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QBuffer>

class Client;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void GetDataFormDatabase();

public slots:
    void FillInfo();

private slots:
    void on_btnConnect_clicked();

    void on_btnRemove_clicked();

    void on_tbDataBase_clicked(const QModelIndex &index);

private:
    std::unique_ptr<Client> pClient;
    Ui::MainWindow *ui;
    QSqlDatabase database;
    QSqlQuery* query;
    QSqlTableModel* model;
    QByteArray data;
    int arrow;
};
#endif // MAINWINDOW_H
