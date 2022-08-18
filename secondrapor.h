#ifndef SECONDRAPOR_H
#define SECONDRAPOR_H

#include <QDialog>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QMessageBox>
#include "mainwindow.h"
#include "tablo.h"

namespace Ui {
class secondRapor;
}

class secondRapor : public QDialog
{
    Q_OBJECT

public:
    explicit secondRapor(QWidget *parent = nullptr);
    ~secondRapor();

private:
    Ui::secondRapor *ui;
    float moneyEur = 0;
    void Init();
    QSqlQueryModel *model;
    void MoneyCalc();

private slots:

    void on_ek_btn_clicked();
};

#endif // SECONDRAPOR_H
