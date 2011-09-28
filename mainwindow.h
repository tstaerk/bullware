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

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
