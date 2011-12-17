#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
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


    QSqlDatabase db;
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("bullware.dat");
    if (db.open())
    {
        qDebug() << "db open: success";
        QSqlQuery query;
        if (query.exec("create table if not exists transactions(stock varchar(20), type int, date date, amount float, price float, currency char(3), sum float, sumcurrency char(3))"))
        {
            qDebug() << "create query successful";
            int line=0;
            if (query.exec(QString("insert into transactions VALUES ('").append(ui->tableWidget->item(line,0)->text()).append("',1,'2011-12-12',1.0,1.0,'DEM',1.0,'DEM')")))
            {
            }
            else
            {
                qDebug() << "insert query failed";
            }
        }
        else
        {
            qDebug() << "create query failed";
        }
    }
    else
        qDebug() << "db open failed";
    db.commit();
    db.close();


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
