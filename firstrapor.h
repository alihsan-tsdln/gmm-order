#ifndef FIRSTRAPOR_H
#define FIRSTRAPOR_H

#include <QDialog>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QMessageBox>

namespace Ui {
class firstRapor;
}

class firstRapor : public QDialog
{
    Q_OBJECT

public:
    explicit firstRapor(QWidget *parent = nullptr);
    ~firstRapor();

private slots:
    void on_ek_btn_clicked();

private:
    Ui::firstRapor *ui;
    void Init();
    QSqlQueryModel *model;
};

#endif // FIRSTRAPOR_H
