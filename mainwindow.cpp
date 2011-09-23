#include <QDebug>
#include <QFile>
#include <QTableWidgetItem>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    QString filename("bullware.csv");
    QFile* file1=new QFile(filename);
    int x;
    if (file1->open(QIODevice::WriteOnly))
    {

        for (x=0; x<ui->tableWidget->columnCount(); ++x)
        {
            file1->write(ui->tableWidget->item(0,x)->text().toAscii());
            qDebug() << x << endl;
            qDebug() << ui->tableWidget->item(0,x)->text().toAscii() << endl;
        }
        file1->close();
    }
    delete ui;
}
