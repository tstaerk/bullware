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
    bool filewriteerror=false;
    QString filename("bullware.csv");
    QFile* file1=new QFile(filename);
    if (file1->open(QIODevice::WriteOnly))
    {
        for (int y=0; y<ui->tableWidget->rowCount(); ++y)
        {
            for (int x=0; x<ui->tableWidget->columnCount(); ++x)
            {
                if (ui->tableWidget->item(y,x))
                {
                    if (file1->write(ui->tableWidget->item(y,x)->text().toAscii())==-1)
                        filewriteerror=true;
                }
            }
            file1->write("\n");
        }
        file1->close();
    }
    else QMessageBox::warning(0,QString("Warning"),QString("Could not open file bullware.csv for writing."));
    if (filewriteerror) QMessageBox::warning(0,QString("Warning"),QString("Could not write to file bullware.csv."));
    delete ui;
}
