#include "sorumlu.h"
#include "ui_sorumlu.h"

Sorumlu::Sorumlu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Sorumlu)
{
    ui->setupUi(this);
}

QString val;

Sorumlu::~Sorumlu()
{
    delete ui;
}

QComboBox *lst;

void Sorumlu::sorumluList(QComboBox *list)
{
    lst = list;
    ui->sorumlu_list->setModel(lst->model());
}

void Sorumlu::on_ekle_btn_clicked()
{
    QSqlQuery query;
    query.prepare("insert into sorumlu values(:ad)");
    query.bindValue(":ad", ui->sorumlu_ln->text());
    if(!query.exec())
    {
        QMessageBox::critical(this, "Hata", query.lastError().text());
    }
    lst->addItem(ui->sorumlu_ln->text());
    ui->sorumlu_list->setModel(lst->model());
}

void Sorumlu::on_sil_btn_clicked()
{
    QSqlQuery query;
    query.exec("delete from sorumlu where ad='" + val + "'");
    lst->removeItem(lst->findText(val));
    ui->sorumlu_list->setModel(lst->model());
}


void Sorumlu::on_sorumlu_list_clicked(const QModelIndex &index)
{
    qDebug() << "Aktif";
    val = ui->sorumlu_list->model()->data(index).toString();
}

