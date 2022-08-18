#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWebSockets/QtWebSockets>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QMessageBox>
#include "tablo.h"
#include "sorumlu.h"
#include "firstrapor.h"
#include "secondrapor.h"
#include "turkishlira.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static double eurUsd;


private slots:

    void on_ekle_btn_clicked();

    void on_sorumlu_btn_clicked();

    void on_veritabani_btn_clicked();

    void on_baslangic_date_userDateChanged(const QDate &date);

    void on_kabul_date_userDateChanged(const QDate &date);

    void on_sevk_date_userDateChanged(const QDate &date);

    void on_birinci_btn_clicked();

    void on_ikinci_btn_clicked();

    void finished(QNetworkReply* reply);

    void on_a_btn_clicked();

    void on_pdf_ekle_btn_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase qsql = QSqlDatabase::addDatabase("QSQLITE");
    QSqlQueryModel *model;
    void Init();
    void moneyCalc();
};
#endif // MAINWINDOW_H
