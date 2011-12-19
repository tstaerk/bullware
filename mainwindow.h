#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString type(QString type);
    void load();
    void savetabletodb();
    void fillfilters();
    void filterlist();
    QString importcsvfile(QString filename);

private slots:
    void on_actionImport_CSV_File_triggered();

    void on_actionQuit_triggered();

    void on_tableWidget_customContextMenuRequested(const QPoint &pos);

    void on_commandLinkButton_clicked();

    void on_commandLinkButton_2_clicked();

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_comboBox_2_currentIndexChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
