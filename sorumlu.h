#ifndef SORUMLU_H
#define SORUMLU_H

#include <QDialog>
#include <QComboBox>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QMessageBox>

namespace Ui {
class Sorumlu;
}

class Sorumlu : public QDialog
{
    Q_OBJECT

public:
    explicit Sorumlu(QWidget *parent = nullptr);
    ~Sorumlu();
    void sorumluList(QComboBox *list);

private slots:
    void on_ekle_btn_clicked();

    void on_sil_btn_clicked();

    void on_sorumlu_list_clicked(const QModelIndex &index);

private:
    Ui::Sorumlu *ui;
};

#endif // SORUMLU_H
