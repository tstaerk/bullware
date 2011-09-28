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
    QString separator(";");
    QFile* file1=new QFile(filename);
    if (file1->open(QIODevice::WriteOnly))
    {
        for (int y=0; y<ui->tableWidget->rowCount(); ++y)
        {
            for (int x=0; x<ui->tableWidget->columnCount(); ++x)
            {
                if (ui->tableWidget->item(y,x))
                {
                    if (file1->write(ui->tableWidget->item(y,x)->text().append(separator).toAscii())==-1)
                        filewriteerror=true;
                }
            }
            file1->write("\n");
        }
        file1->close();
    }
    else QMessageBox::warning(0,QString("Warning"),QString("Could not open file bullware.csv for writing."));
    if (filewriteerror) QMessageBox::warning(0,QString("Warning"),QString("Could not write to file bullware.csv."));
    delete file1;
    delete ui;
}

QString MainWindow::importcsvfile(QString filename)
{
    QString err;
    QFile* file1=new QFile(filename);
    if (file1->open(QIODevice::ReadOnly))
    {

        QString separator(";");
        QString filecontent=file1->readAll();
        QStringList rows=filecontent.split("\n");
        ui->tableWidget->setRowCount(rows.count()+1);
        for (int y=0; y<rows.count(); y++)
        {
            QStringList list=rows[y].split(separator);
            for (int x=0; x<list.count(); x++)
            {
                delete ui->tableWidget->item(y,x);
                ui->tableWidget->setItem(y,x,new QTableWidgetItem(list[x].replace("\"","").replace("\n","")));
            }
        }
    }
    else
        QMessageBox::warning(0,"Attention","Could not open file.");
    delete file1;
    return QString();
}

void MainWindow::on_actionImport_CSV_File_triggered()
{
    importcsvfile(QFileDialog::getOpenFileName(0,"Enter a transactions file."));
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}
