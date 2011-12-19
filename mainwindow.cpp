/* Concept

  All is stored as transactions. The sum of available stocks is calculated by adding buy actions and subtracting sell actions.

  Examples for transactions:
  you buy $1000 for 800EUR
  you get a dividend of $50 from your Microsoft stocks
  you find you have an inventory of 50 shares of fund ABC
  you have to pay $20 as deposit fees for stock XYZ
  you pay 0.12 funds of fund ABC for deposit fees
  you save EUR 100 on a bank account
  you withdraw EUR 100 from a bank account
  interests from your bank account go to your bank account
  your fund shares accumulate because you held them for one year
  you have to separate your inventory of fund ABC for tax reasons. deposit 00 has 100 shares, deposit 01 has 200 shares.
  as an employee you get 1 stock from your company as an incentive
  you bought stocks XYZ in 1988 but cannot remember the exact date

  So a transaction can be of type
  1 - buy
  2 - sell
  3 - inventory
  4 - dividend
  5 - accumulation
  6 - monetary deposit fees
  7 - fund deposit fees
  8 - save
  9 - withdraw
  10 - interests

  Different currencies are possible.

  Security
  Against data damage: none. If you use sql injections to destroy your own data, you want it so.

*/
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
    //importcsvfile("bullware.csv");
    load();
    // Add options to filter combo box
    fillfilters();
}

MainWindow::~MainWindow()
{
    bool filewriteerror=false;
    QString filename("bullware.csv");
    QString separator(";");
    QFile* file1=new QFile(filename);
    if (file1->open(QIODevice::WriteOnly))
    {
        for (int y=0; y<ui->tableWidget->rowCount()-1; ++y)
        {
            for (int x=0; x<ui->tableWidget->columnCount(); ++x)
            {
                if (ui->tableWidget->item(y,x))
                {
                    if (file1->write(ui->tableWidget->item(y,x)->text().toAscii())==-1)
                        filewriteerror=true;
                }
                if (file1->write((separator).toAscii())==-1)
                    filewriteerror=true;
                qDebug() << "writing ;" << "y=" << y;
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

QString MainWindow::type(QString type)
{
    QString result="0";
    if (type=="buy") result="1";
    if (type=="sell") result="2";
    if (type=="inventory") result="3";
    if (type=="dividend") result="4";
    if (type=="accumulation") result="5";
    return result;
}

void MainWindow::load()
{
    QSqlDatabase db;
    int y=0;
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("bullware.dat");
    ui->tableWidget->setRowCount(1);
    if (db.open())
    {
        QSqlQuery query;
        if (query.exec("select * from transactions"))
        {
            if (!query.first()) qDebug() << "no result from query";
            else
            {
                ui->tableWidget->setItem(0,0,new QTableWidgetItem(query.value(0).toString()));
                while (query.next())
                {
                    ui->tableWidget->setItem(y,0,new QTableWidgetItem(query.value(0).toString()));
                    ui->tableWidget->setItem(y,1,new QTableWidgetItem(query.value(1).toString()));
                    ui->tableWidget->setItem(y,2,new QTableWidgetItem(query.value(2).toString()));
                    ui->tableWidget->setItem(y,3,new QTableWidgetItem(query.value(3).toString()));
                    y++;
                    qDebug() << "query" << query.value(1).toString();
                    ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
                }
            }
        }
        else qDebug() << "could not query";
    }
    else qDebug() << "could not open database";
    db.close();
}

void MainWindow::savetabletodb()
{
    QSqlDatabase db;
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("bullware.dat");
    if (db.open())
    {
        qDebug() << "db open: success";
        QSqlQuery query;
        query.exec("drop table transactions");
        if (query.exec("create table if not exists transactions(stock varchar(20), type int, date date, amount float, price float, currency char(3), sum float, sumcurrency char(3))"))
        {
            qDebug() << "create query successful";
            for (int y=0; y<ui->tableWidget->rowCount()-1; ++y)
            {
                if (ui->tableWidget->item(y,0))
                {
                    QString line="'";
                    line.append(ui->tableWidget->item(y,0)->text());
                    line.append("',");
                    if (ui->tableWidget->item(y,1)->text()=="buy") line.append("1");
                    else if (ui->tableWidget->item(y,1)->text()=="sell") line.append("2");
                    else if (ui->tableWidget->item(y,1)->text()=="inventory") line.append("3");
                    else if (ui->tableWidget->item(y,1)->text()=="dividend") line.append("4");
                    else if (ui->tableWidget->item(y,1)->text()=="accumulation") line.append("5");
                    else line.append("0");
                    line.append(",'");
                    line.append(ui->tableWidget->item(y,2)->text());
                    line.append("',");
                    line.append(ui->tableWidget->item(y,3)->text());
                    line.append(",");
                    if (!query.exec(QString("insert into transactions VALUES (").append(line).append("1.0,'DEM',1.0,'DEM')")))
                    {
                        qDebug() << "insert query failed";
                    }
                }
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
}

void MainWindow::fillfilters()
{
    // Add options to filter combo box
    QSqlDatabase db;
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("bullware.dat");
    if (db.open())
    {
        QSqlQuery query;
        if (query.exec("select distinct stock from transactions order by stock"))
        {
            if (!query.first()) qDebug() << "no result from query";
            ui->comboBox->addItem(query.value(0).toString());
            while (query.next())
            {
                ui->comboBox->addItem(query.value(0).toString());
            }
        }
        else qDebug() << "could not query";
    }
    else qDebug() << "could not open database";
    db.close();
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
        ui->tableWidget->setRowCount(rows.count());
        for (int y=0; y<rows.count()-1; y++)
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
    savetabletodb();
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

void MainWindow::on_tableWidget_customContextMenuRequested(const QPoint &pos)
{
    QMenu* menu1=new QMenu(0);
    menu1->addAction("delete");
    menu1->popup(QPoint(1,1));
}

void MainWindow::on_commandLinkButton_clicked()
{
    ui->tableWidget->removeRow(ui->tableWidget->currentRow());
}

void MainWindow::on_commandLinkButton_2_clicked()
{
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
}

void MainWindow::filterlist()
{
    while (ui->tableWidget_2->rowCount())
        ui->tableWidget_2->removeRow(0);
    ui->tableWidget_2->setColumnCount(9);
    QSqlDatabase db;
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("bullware.dat");
    if (db.open())
    {
        QSqlQuery query; // class that is needed around the query to query
        QString query2;  // the actual query
        if (ui->comboBox->currentText()=="All Stocks")
        {
            if (ui->comboBox_2->currentIndex()==0)
                query2="select * from transactions";
            else
            {
                query2="select * from transactions where type='";
                query2.append(type(ui->comboBox_2->currentText())).append("'");
            }
        }
        else
        {
            if (ui->comboBox_2->currentIndex()==0)
            {
                query2="select * from transactions where stock='";
                query2.append(ui->comboBox->currentText()).append("'");
            }
            else
            {
                query2="select * from transactions where stock='";
                query2.append(ui->comboBox->currentText()).append("' and type='");
                query2.append(type(ui->comboBox_2->currentText())).append("'");
            }
        }
        qDebug() << "query2 = " << query2;
        if (query.exec(query2))
        {
            if (!query.first()) qDebug() << "no result from query";
            qDebug() << "query delivers" << query.value(0).toString();
            ui->tableWidget_2->insertRow(0);
            ui->tableWidget_2->setItem(0,0,new QTableWidgetItem(query.value(0).toString()));
            while (query.next())
            {
                ui->tableWidget_2->insertRow(0);
                ui->tableWidget_2->setItem(0,0,new QTableWidgetItem(query.value(0).toString()));
            }
        }
        else qDebug() << "could not query";
    }
    else qDebug() << "could not open database";
    db.close();
}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    filterlist();
}

void MainWindow::on_comboBox_2_currentIndexChanged(const QString &arg1)
{
    filterlist();
}
