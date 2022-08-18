#ifndef TURKISHLIRA_H
#define TURKISHLIRA_H

#include <QMainWindow>
#include "mainwindow.h"

class turkishLira : public QMainWindow
{
    Q_OBJECT
public:
    explicit turkishLira(QWidget *parent = nullptr);
    static double eurTl;
private slots:
    void finished(QNetworkReply *reply);
private:
    void Init();

signals:

};

#endif // TURKISHLIRA_H
