#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlRecord>

class MyServer;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void FillDatabase();

private slots:
    void on_btnSend_clicked();

private:
    Ui::MainWindow *ui;
    std::unique_ptr<MyServer> server;
    QSqlDatabase database;
    QSqlQuery* query;
    QSqlTableModel* model;
};
#endif // MAINWINDOW_H
