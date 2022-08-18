#include "tablo.h"
#include "ui_tablo.h"
#include <QFileDialog>
#include <QAxObject>
#include <QDesktopServices>

Tablo::Tablo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Tablo)
{
    ui->setupUi(this);
    Init();
}

Tablo::~Tablo()
{
    delete ui;
}

float moneyEur = 0;

void Tablo::Init()
{
    moneyCalc();
    if(turkishLira::eurTl != 0 && MainWindow::eurUsd != 0)
    {
        moneyCalc();
    }
}

void Tablo::moneyCalc()
{
    QSqlQuery query;
    query.exec("select * from tablo");
    moneyEur = 0;

    while(query.next())
    {
        QString val = query.value(8).toString();

        if(val.contains("€"))
        {
            val.remove("€");
            moneyEur += val.toFloat();
        }

        else if(val.contains("$"))
        {
            val.remove("$");
            moneyEur += val.toFloat() / MainWindow::eurUsd;
        }
    }

    model = new QSqlQueryModel(this);
    model->setQuery(std::move(query));
    ui->tablo_table->setModel(model);
    ui->tutar_lbl->setText("Tutar :  " + QString::number(moneyEur) + "€    Tutar  : " + QString::number(moneyEur * turkishLira::eurTl) + "₺");
}

QString val2;

void Tablo::on_tablo_table_clicked(const QModelIndex &index)
{
    val2 = ui->tablo_table->model()->data(ui->tablo_table->model()->index(index.row(), 0)).toString();
}

void Tablo::on_pushButton_clicked()
{
    QSqlQuery query;

    query.exec("delete from tablo where id = '" + val2 + "'");

    QSqlQuery sorgu;
    query.exec("select * from tablo");
    model = new QSqlQueryModel();
    model->setQuery(std::move(sorgu));
    ui->tablo_table->setModel(model);
    ui->ek_btn->setEnabled(true);
    moneyCalc();
}


void Tablo::on_ek_btn_clicked()
{
    QString filepath=QFileDialog::getSaveFileName( this ,tr("Save orbit"), "." ,tr( "Microsoft Office Excel 2007 (*.xlsx)" )); //Get the save path

    if (!filepath.isEmpty()){
        QAxObject *excel = new QAxObject(this);
        excel->setControl("Excel.Application"); //Connect to Excel control
        excel->dynamicCall("SetVisible (bool Visible)", false); //Do not display the form
        excel->setProperty("DisplayAlerts" , false); //Do not display any warning messages. If it is true, a prompt similar to "File has been modified, whether to save" will appear when closing
        QAxObject *workbooks = excel->querySubObject("WorkBooks"); //Get workbook collection
        workbooks->dynamicCall("Add"); //Create a new workbook
        QAxObject *workbook = excel->querySubObject("ActiveWorkBook"); //Get the current workbook
        QAxObject *worksheets = workbook->querySubObject("Sheets"); //Get worksheet collection
        QAxObject *worksheet = worksheets->querySubObject("Item(int)" ,1); //Get worksheet 1 of the worksheet collection, namely sheet1
        QAxObject *cellX;

        QString X="A"+QString::number(1);
        cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
        cellX->dynamicCall("SetValue(const QVariant&)",QVariant("Müşteri Adı"));
        X = "B"+QString::number(1);
        cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
        cellX->dynamicCall("SetValue(const QVariant&)",QVariant("Proje Kodu"));
        X = "C"+QString::number(1);
        cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
        cellX->dynamicCall("SetValue(const QVariant&)",QVariant("Proje Sorumlusu"));
        X = "D"+QString::number(1);
        cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
        cellX->dynamicCall("SetValue(const QVariant&)",QVariant("Makine Tipi"));
        X = "E"+QString::number(1);
        cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
        cellX->dynamicCall("SetValue(const QVariant&)",QVariant("Marka"));
        X = "F"+QString::number(1);
        cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
        cellX->dynamicCall("SetValue(const QVariant&)",QVariant("Durum"));
        X = "G"+QString::number(1);
        cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
        cellX->dynamicCall("SetValue(const QVariant&)",QVariant("Adet"));
        X = "H"+QString::number(1);
        cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
        cellX->dynamicCall("SetValue(const QVariant&)",QVariant("Tutar (€)"));
        X = "I"+QString::number(1);
        cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
        cellX->dynamicCall("SetValue(const QVariant&)",QVariant("Sipariş Başlangıç Tarihi"));
        X = "J"+QString::number(1);
        cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
        cellX->dynamicCall("SetValue(const QVariant&)",QVariant("Ön Kabul Tarihi"));
        X = "K"+QString::number(1);
        cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
        cellX->dynamicCall("SetValue(const QVariant&)",QVariant("Sevk Tarihi"));
        X = "L"+QString::number(1);
        cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
        cellX->dynamicCall("SetValue(const QVariant&)",QVariant("Teslim Tarihi"));
        X = "M"+QString::number(1);
        cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
        cellX->dynamicCall("SetValue(const QVariant&)",QVariant("Tamamlanma Yüzdesi"));

        for(int i = 2; i < ui->tablo_table->model()->rowCount() + 2; i++)
        {
            QString X="A"+QString::number(i);
            cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
            cellX->dynamicCall("SetValue(const QVariant&)",QVariant(ui->tablo_table->model()->data(ui->tablo_table->model()->index(i - 2, 1)).toString()));
            X = "B"+QString::number(i);
            cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
            cellX->dynamicCall("SetValue(const QVariant&)",QVariant(ui->tablo_table->model()->data(ui->tablo_table->model()->index(i - 2, 2)).toString()));
            X = "C"+QString::number(i);
            cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
            cellX->dynamicCall("SetValue(const QVariant&)",QVariant(ui->tablo_table->model()->data(ui->tablo_table->model()->index(i - 2, 3)).toString()));
            X = "D"+QString::number(i);
            cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
            cellX->dynamicCall("SetValue(const QVariant&)",QVariant(ui->tablo_table->model()->data(ui->tablo_table->model()->index(i - 2, 4)).toString()));
            X = "E"+QString::number(i);
            cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
            cellX->dynamicCall("SetValue(const QVariant&)",QVariant(ui->tablo_table->model()->data(ui->tablo_table->model()->index(i - 2, 5)).toString()));
            X = "F"+QString::number(i);
            cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
            cellX->dynamicCall("SetValue(const QVariant&)",QVariant(ui->tablo_table->model()->data(ui->tablo_table->model()->index(i - 2, 6)).toString()));
            X = "G"+QString::number(i);
            cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
            cellX->dynamicCall("SetValue(const QVariant&)",QVariant(ui->tablo_table->model()->data(ui->tablo_table->model()->index(i - 2, 7)).toString()));
            X = "H"+QString::number(i);
            QString temp = ui->tablo_table->model()->data(ui->tablo_table->model()->index(i - 2, 8)).toString();
            if(temp.contains("$"))
            {
                temp.remove("$");
                temp = QString::number(temp.toFloat() / MainWindow::eurUsd);
            }

            else if(temp.contains("€"))
            {
                temp.remove("€");
            }
            cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
            cellX->dynamicCall("SetValue(const QVariant&)", QVariant(temp));
            X = "I"+QString::number(i);
            cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
            cellX->dynamicCall("SetValue(const QVariant&)",QVariant(ui->tablo_table->model()->data(ui->tablo_table->model()->index(i - 2, 9)).toString()));
            X = "J"+QString::number(i);
            cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
            cellX->dynamicCall("SetValue(const QVariant&)",QVariant(ui->tablo_table->model()->data(ui->tablo_table->model()->index(i - 2, 10)).toString()));
            X = "K"+QString::number(i);
            cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
            cellX->dynamicCall("SetValue(const QVariant&)",QVariant(ui->tablo_table->model()->data(ui->tablo_table->model()->index(i - 2, 11)).toString()));
            X = "L"+QString::number(i);
            cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
            cellX->dynamicCall("SetValue(const QVariant&)",QVariant(ui->tablo_table->model()->data(ui->tablo_table->model()->index(i - 2, 12)).toString()));
            X = "M"+QString::number(i);
            cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
            cellX->dynamicCall("SetValue(const QVariant&)",QVariant(ui->tablo_table->model()->data(ui->tablo_table->model()->index(i - 2, 13)).toString()));
        }

        //G-H
        X = "G"+QString::number(ui->tablo_table->model()->rowCount() + 2);
        cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
        cellX->dynamicCall("SetValue(const QVariant&)",QVariant("Toplam (€) :"));
        X = "H"+QString::number(ui->tablo_table->model()->rowCount() + 2);
        cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
        cellX->dynamicCall("SetValue(const QVariant&)",QVariant(QString::number(moneyEur)));
        X = "G"+QString::number(ui->tablo_table->model()->rowCount() + 3);
        cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
        cellX->dynamicCall("SetValue(const QVariant&)",QVariant("Toplam (₺) :"));
        X = "H"+QString::number(ui->tablo_table->model()->rowCount() + 3);
        cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
        cellX->dynamicCall("SetValue(const QVariant&)",QVariant(QString::number(moneyEur * turkishLira::eurTl)));


        workbook->dynamicCall("SaveAs(const QString&)",QDir::toNativeSeparators(filepath));//Save to filepath, be sure to use QDir::toNativeSeparators to convert the "/" in the path to "\", otherwise It must not be saved.
        workbook->dynamicCall("Close()");//Close the workbook
        excel->dynamicCall("Quit()");//Close excel
        delete excel;
        excel=NULL;
    }
}


void Tablo::on_pdf_btn_clicked()
{
    if(!QFile::exists(QString("./temp/") + val2 + QString(".pdf")))
    {
        QMessageBox::information(this, "PDF Bulunmamakta", "İstediğiniz PDF'i eklemediniz.");
        return;
    }



    QDesktopServices::openUrl(QUrl::fromLocalFile(QString("./temp/") + val2 + QString(".pdf")));
}


void Tablo::on_pdf_ekle_btn_clicked()
{
    QSqlQuery query;
    QString filepath = "";

    filepath = QFileDialog::getOpenFileName(this,tr("PDF Ekle"), ".", tr( "PDF (*.pdf)"));

    if(filepath.isEmpty())
    {
        return;
    }

    QDir dir;

    if(!dir.exists("./temp"))
    {
        dir.mkdir("./temp");
    }

    if(filepath.isEmpty())
        return;

    QFile::copy(filepath, QString("./temp/")+ val2 + QString(".pdf"));
}

