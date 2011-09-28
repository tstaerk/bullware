#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTableWidgetItem>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    importcsvfile("bullware.csv");
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

QString MainWindow::importcsvfile(QString filename)
{
    QFile* file1=new QFile(filename);
    if (file1->open(QIODevice::ReadOnly))
    {
        int y=-1;
        QString separator(";");
        while (!file1->atEnd())
        {
            y++;
            QString line=file1->readLine();
            qDebug() << line;
            QStringList list=line.split(separator);
            qDebug() << ui->tableWidget->item(0,0);
            for (int x=0; x<list.count(); x++)
            {
                delete ui->tableWidget->item(y,x);
                ui->tableWidget->setItem(y,x,new QTableWidgetItem(list[x].replace("\"","").replace("\n","")));
            }
        }
    }
    else
        QMessageBox::warning(0,"Attention","Could not open file.");
    return QString();
}

void MainWindow::on_actionImport_CSV_File_triggered()
{
    importcsvfile(QFileDialog::getOpenFileName(0,"Enter a transactions file."));
}
