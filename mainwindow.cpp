#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

double MainWindow::eurUsd;


void MainWindow::Init()
{
    qDebug() << QDir::fromNativeSeparators(QDir::tempPath());
    ui->baslangic_date->setMinimumDate(QDate::currentDate());
    ui->kabul_date->setMinimumDate(QDate::currentDate().addDays(1));
    ui->sevk_date->setMinimumDate(QDate::currentDate().addDays(2));
    ui->teslim_date->setMinimumDate(QDate::currentDate().addDays(3));

    qsql.setDatabaseName("C:/Users/alihsan-tsdln/Desktop/Coding/VisualProgramming/GMM_Veritaban/sql.db");

    if(!qsql.open())
        ui->statusbar->showMessage("Veritabanına Bağlanılamadı");

    else
        ui->statusbar->showMessage("Veritabanına Bağlanıldı");

    moneyCalc();

    QSqlQuery query;
    query.exec("select * from sorumlu");
    while(query.next())
    {
        ui->proje_sorumlusu_cmb->addItem(query.value(0).toString());
    }

    QSqlQuery qry;
    qry.prepare("select id, siparis_baslangic_tarihi, teslim_tarihi from tablo");

    if(qry.exec())
    {
        while(qry.next())
        {
            qry.exec("update tablo set tamamlanma_yuzdesi = '" + QString::number(100 / QDate::fromString(qry.value(1).toString()).daysTo(QDate::fromString(qry.value(2).toString())) * QDate::fromString(qry.value(1).toString()).daysTo(QDate::currentDate())).append("%") + "' where id = '" + qry.value(0).toString() + "'");
        }
    }

    else
        QMessageBox::critical(this, "Hata", qry.lastError().text());


}

void MainWindow::moneyCalc()
{
    QNetworkAccessManager *manager;
    QString url = "https://free.currconv.com/api/v7/convert?q=EUR_USD&compact=ultra&apiKey=45690ddeb55b52ed309c";

    manager = new QNetworkAccessManager(this);  // Create a class object that is the center of the API
    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));

    QNetworkRequest request= QNetworkRequest(QUrl(url));// Create a QNetworkRequest object with the url address as the requested url address
    manager->get(request);

}

void MainWindow::finished(QNetworkReply* reply)
{
    QByteArray response = reply->readAll();
    QString a = QString(response);
    a = a.mid(11,7);
    eurUsd = a.toFloat();
    new turkishLira(this);
    qDebug() << eurUsd;
}

QString filepath = "";

void MainWindow::on_ekle_btn_clicked()
{
    if(ui->musteri_adi_ln->text().isEmpty())
    {
        QMessageBox::information(this, "Girilmeyen Veri", "Lütfen Müşteri Adı kısmını doldurunuz.");
        return;
    }

    else if(ui->proje_kodu_ln->text().isEmpty())
    {
        QMessageBox::information(this, "Girilmeyen Veri", "Lütfen Proje Kodu kısmını doldurunuz.");
        return;
    }

    else if(ui->makine_tipi_ln->text().isEmpty())
    {
        QMessageBox::information(this, "Girilmeyen Veri", "Lütfen Makine Tipi kısmını doldurunuz.");
        return;
    }

    else if(ui->tutar_ln->text().isEmpty())
    {
        QMessageBox::information(this, "Girilmeyen Veri", "Lütfen Tutar kısmını doldurunuz.");
        return;
    }

    QString musteri_adi = ui->musteri_adi_ln->text();
    QString proje_kodu = ui->proje_kodu_ln->text();
    QString proje_sorumlusu = ui->proje_sorumlusu_cmb->currentText();
    QString makine_tipi = ui->makine_tipi_ln->text();
    QString marka = ui->marka_cmb->currentText();
    QString durum = ui->durum_cmb->currentText();
    int adet = ui->adet_spn->value();
    QString tutar = ui->tutar_ln->text();
    while(tutar.contains(" "))
        tutar.remove(" ");

    if(tutar.contains("$") && tutar.contains("€"))
    {
        QMessageBox::critical(this, "Para Birimi",
                              "İki Para birimini aynı anda yazamazsınız.");
        return;
    }

    else if(!(tutar.contains("$") || tutar.contains("€")))
    {
        QMessageBox::critical(this, "Para Birimi",
                              "Para birimini giriniz.");
        return;
    }

    else if(tutar.contains("$"))
    {
        while(tutar.contains("$"))
            tutar.remove("$");

        bool ok;      // hex == 255, ok == true
        tutar.toInt(&ok);

        if(!ok)
        {
            QMessageBox::information(this, "Yanlış Tutar", "Lütfen tutarı tamamen sayısal giriniz");
            return;
        }

        tutar.append("$");
    }

    else if(tutar.contains("€"))
    {
        while(tutar.contains("€"))
            tutar.remove("€");

        bool ok;      // hex == 255, ok == true
        tutar.toInt(&ok);

        if(!ok)
        {
            QMessageBox::information(this, "Yanlış Tutar", "Lütfen tutarı tamamen sayısal giriniz");
            return;
        }

        tutar.append("€");
    }

    QString siparis_baslangic_tarihi = ui->baslangic_date->date().toString();
    QString on_kabul_tarihi = ui->kabul_date->date().toString();
    QString sevk_tarihi = ui->sevk_date->date().toString();
    QString teslim_tarihi = ui->teslim_date->date().toString();

    QSqlQuery query;
    query.prepare("insert into tablo values(:id, :musteri_adi, :proje_kodu, :proje_sorumlusu, :makine_tipi, :marka, :durum, :adet, :tutar, :siparis_baslangic_tarihi, :on_kabul_tarihi, :sevk_tarihi, :teslim_tarihi, :tamamlanma_yuzdesi)");
    query.bindValue(":musteri_adi", musteri_adi);
    query.bindValue(":proje_kodu", proje_kodu);
    query.bindValue(":proje_sorumlusu", proje_sorumlusu);
    query.bindValue(":makine_tipi", makine_tipi);
    query.bindValue(":marka", marka);
    query.bindValue(":durum", durum);
    query.bindValue(":adet", adet);
    query.bindValue(":tutar", tutar);
    query.bindValue(":siparis_baslangic_tarihi", siparis_baslangic_tarihi);
    query.bindValue(":on_kabul_tarihi", on_kabul_tarihi);
    query.bindValue(":sevk_tarihi", sevk_tarihi);
    query.bindValue(":teslim_tarihi", teslim_tarihi);
    query.bindValue(":tamamlanma_yuzdesi", QString::number(100 / ui->baslangic_date->date().daysTo(ui->teslim_date->date()) * ui->baslangic_date->date().daysTo(QDate::currentDate())).append("%"));
    if(!query.exec())
    {
        QMessageBox::critical(this, "Hata", query.lastError().text());
    }

    QString id;
    query.exec("select id from tablo");
    while(query.next())
    {
        id = query.value(0).toString();
    }

    QDir dir;

    if(!dir.exists("./temp"))
    {
        dir.mkdir("./temp");
    }

    if(filepath.isEmpty())
        return;

    QFile::copy(filepath, QString("./temp/")+ id + QString(".pdf"));
}

void MainWindow::on_sorumlu_btn_clicked()
{
    Sorumlu *srml = new Sorumlu(this);
    srml->sorumluList(ui->proje_sorumlusu_cmb);
    srml->show();
}

void MainWindow::on_veritabani_btn_clicked()
{
    Tablo *tablo = new Tablo();
    tablo->show();
}


void MainWindow::on_baslangic_date_userDateChanged(const QDate &date)
{
    ui->kabul_date->setMinimumDate(date.addDays(1));
}


void MainWindow::on_kabul_date_userDateChanged(const QDate &date)
{
    ui->sevk_date->setMinimumDate(date.addDays(1));
}


void MainWindow::on_sevk_date_userDateChanged(const QDate &date)
{
    ui->teslim_date->setMinimumDate(date.addDays(1));
}


void MainWindow::on_birinci_btn_clicked()
{
    firstRapor *fr = new firstRapor();
    fr->show();
}


void MainWindow::on_ikinci_btn_clicked()
{
    secondRapor *sr = new secondRapor();
    sr->show();
}


void MainWindow::on_a_btn_clicked()
{
}




void MainWindow::on_pdf_ekle_btn_clicked()
{
    //QString filepath = QFileDialog::getSaveFileName( this ,tr("Save orbit"), "." ,tr( "Microsoft Office Excel 2007 (*.xlsx)" )); //Get the save path
    filepath = QFileDialog::getOpenFileName(this,tr("PDF Ekle"), ".", tr( "PDF (*.pdf)"));
}

