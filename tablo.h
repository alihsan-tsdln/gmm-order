#ifndef TABLO_H
#define TABLO_H

#include <QDialog>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QMessageBox>
#include "mainwindow.h"
#include "turkishlira.h"

namespace Ui {
class Tablo;
}

class Tablo : public QDialog
{
    Q_OBJECT

public:
    explicit Tablo(QWidget *parent = nullptr);
    ~Tablo();
    static double eurTl;

private:
    Ui::Tablo *ui;
    QSqlQueryModel *model;
    float moneyEur = 0;
    void Init();
    void moneyCalc();

private slots:
    void on_ek_btn_clicked();
    void on_pushButton_clicked();
    void on_tablo_table_clicked(const QModelIndex &index);
    void on_pdf_btn_clicked();
    void on_pdf_ekle_btn_clicked();
};

#endif // TABLO_H
