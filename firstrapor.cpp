#include "firstrapor.h"
#include "ui_firstrapor.h"
#include <QFileDialog>
#include <QAxObject>

firstRapor::firstRapor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::firstRapor)
{
    ui->setupUi(this);
    Init();
}

firstRapor::~firstRapor()
{
    delete ui;
}

void firstRapor::Init()
{
    QSqlQuery query;
    query.exec("select proje_sorumlusu, musteri_adi, makine_tipi, sevk_tarihi from tablo");
    model = new QSqlQueryModel();
    model->setQuery(std::move(query));
    ui->birinci_table->setModel(model);
}

void firstRapor::on_ek_btn_clicked()
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
        cellX->dynamicCall("SetValue(const QVariant&)",QVariant("Proje Sorumlusu"));
        X = "B"+QString::number(1);
        cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
        cellX->dynamicCall("SetValue(const QVariant&)",QVariant("Müşteri Adı"));
        X="C"+QString::number(1);
        cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
        cellX->dynamicCall("SetValue(const QVariant&)",QVariant("Makine Tipi"));
        X = "D"+QString::number(1);
        cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
        cellX->dynamicCall("SetValue(const QVariant&)",QVariant("Sevk Tarihi"));

        //ui->birinci_table->model()->data(ui->birinci_table->model()->index(i - 2, 0))


        qDebug() << ui->birinci_table->model()->rowCount();

        for(int i = 2; i < ui->birinci_table->model()->rowCount() + 2; i++)
        {
            QString X="A"+QString::number(i);
            cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
            cellX->dynamicCall("SetValue(const QVariant&)",QVariant(ui->birinci_table->model()->data(ui->birinci_table->model()->index(i - 2, 0))));
            X = "B"+QString::number(i);
            cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
            cellX->dynamicCall("SetValue(const QVariant&)",QVariant(ui->birinci_table->model()->data(ui->birinci_table->model()->index(i - 2, 1))));
            X="C"+QString::number(i);
            cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
            cellX->dynamicCall("SetValue(const QVariant&)",QVariant(ui->birinci_table->model()->data(ui->birinci_table->model()->index(i - 2, 2))));
            X = "D"+QString::number(i);
            cellX = worksheet->querySubObject("Range(QVariant, QVariant)",X);
            cellX->dynamicCall("SetValue(const QVariant&)",QVariant(ui->birinci_table->model()->data(ui->birinci_table->model()->index(i - 2, 3))));

        }

        workbook->dynamicCall("SaveAs(const QString&)",QDir::toNativeSeparators(filepath));//Save to filepath, be sure to use QDir::toNativeSeparators to convert the "/" in the path to "\", otherwise It must not be saved.
        workbook->dynamicCall("Close()");//Close the workbook
        excel->dynamicCall("Quit()");//Close excel
        delete excel;
        excel=NULL;
        }
}

