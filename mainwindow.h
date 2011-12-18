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
    QString importcsvfile(QString filename);

private slots:
    void on_actionImport_CSV_File_triggered();

    void on_actionQuit_triggered();

    void on_tableWidget_customContextMenuRequested(const QPoint &pos);

    void on_commandLinkButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
