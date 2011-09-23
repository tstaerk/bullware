#include <QDebug>
#include <QFile>
#include <QMessageBox>
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
    if (file1->open(QIODevice::WriteOnly))
    {
        for (int x=0; x<ui->tableWidget->columnCount(); ++x)
        {
            if (ui->tableWidget->item(0,x))
            {
                file1->write(ui->tableWidget->item(0,x)->text().toAscii());
            }
        }
        file1->close();
    }
    else QMessageBox::warning(0,QString("Warning"),QString("Could not open file bullware.csv for writing."));
    delete ui;
}
