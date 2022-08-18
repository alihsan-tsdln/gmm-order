#include "secondrapor.h"
#include "ui_secondrapor.h"
#include <QAxObject>
#include <QFileDialog>

secondRapor::secondRapor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::secondRapor)
{
    ui->setupUi(this);
    Init();
}

secondRapor::~secondRapor()
{
    delete ui;
}

void secondRapor::Init()
{
    //2 line delete
    MoneyCalc();
    ui->ek_btn->setEnabled(true);
    if(MainWindow::eurUsd != 0 && turkishLira::eurTl != 0)
    {
        MoneyCalc();
        ui->tutar_lbl->setText("Tutar :  " + QString::number(moneyEur) + "€    Tutar  : " + QString::number(moneyEur * turkishLira::eurTl) + "₺");
        ui->ek_btn->setEnabled(true);
    }
}

void secondRapor::MoneyCalc()
{
    QSqlQuery query;
    query.exec("select musteri_adi, tutar from tablo");
    while(query.next())
    {
        QString val = query.value(1).toString();
        if(val.contains("$"))
        {
            val.remove("$");
            moneyEur += val.toFloat() / MainWindow::eurUsd;
        }

        else if(val.contains("€"))
        {
            val.remove("€");
            moneyEur += val.toFloat();
        }
    }

    model = new QSqlQueryModel();
    model->setQuery(std::move(query));
    ui->ikinci_table->setModel(model);
}



void secondRapor::on_ek_btn_clicked()
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
        cellX->dynamicCall("SetValue(const QVariant&)",QVariant("Tutar (€)"));

        qDebug() << ui->ikinci_table->model()->rowCount();

        for(int i = 2; i < ui->ikinci_table->model()->rowCount() + 2; i++)
        {
            QString X="A"+QString::number(i);
            cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
            cellX->dynamicCall("SetValue(const QVariant&)",QVariant(ui->ikinci_table->model()->data(ui->ikinci_table->model()->index(i - 2, 0))));
            X="B"+QString::number(i);
            cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
            QString temp = ui->ikinci_table->model()->data(ui->ikinci_table->model()->index(i - 2, 1)).toString();
            if(temp.contains("$"))
            {
                temp.remove("$");
                temp = QString::number(temp.toFloat() / MainWindow::eurUsd);
            }

            else if(temp.contains("€"))
            {
                temp.remove("€");
            }
            cellX->dynamicCall("SetValue(const QVariant&)", QVariant(temp));
        }

        X="A"+QString::number(ui->ikinci_table->model()->rowCount() + 2);
        cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
        cellX->dynamicCall("SetValue(const QVariant&)",QVariant("Toplam (€) :"));
        X = "B"+QString::number(ui->ikinci_table->model()->rowCount() + 2);
        cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
        cellX->dynamicCall("SetValue(const QVariant&)",QVariant(QString::number(moneyEur)));
        X="A"+QString::number(ui->ikinci_table->model()->rowCount() + 3);
        cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
        cellX->dynamicCall("SetValue(const QVariant&)",QVariant("Toplam (₺) :"));
        X = "B"+QString::number(ui->ikinci_table->model()->rowCount() + 3);
        cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
        cellX->dynamicCall("SetValue(const QVariant&)",QVariant(QString::number(moneyEur * turkishLira::eurTl)));

        workbook->dynamicCall("SaveAs(const QString&)",QDir::toNativeSeparators(filepath));//Save to filepath, be sure to use QDir::toNativeSeparators to convert the "/" in the path to "\", otherwise It must not be saved.
        workbook->dynamicCall("Close()");//Close the workbook
        excel->dynamicCall("Quit()");//Close excel
        delete excel;
        excel=NULL;


    }

}

