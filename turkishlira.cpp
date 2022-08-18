#include "turkishlira.h"

turkishLira::turkishLira(QWidget *parent)
    : QMainWindow{parent}
{
    Init();
}

double turkishLira::eurTl;

void turkishLira::finished(QNetworkReply *reply)
{
    QByteArray response = reply->readAll();
    QString a = QString(response);
    a = a.mid(11,8);
    eurTl = a.toFloat();
    qDebug() << eurTl;
}

void turkishLira::Init()
{
    QNetworkAccessManager *manager;
    QString url = "https://free.currconv.com/api/v7/convert?q=EUR_TRY&compact=ultra&apiKey=45690ddeb55b52ed309c";

    manager = new QNetworkAccessManager(this);  // Create a class object that is the center of the API
    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));

    QNetworkRequest request= QNetworkRequest(QUrl(url));// Create a QNetworkRequest object with the url address as the requested url address
    manager->get(request);
}
