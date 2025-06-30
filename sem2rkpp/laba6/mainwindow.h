#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionSign_In_triggered();

    void on_actionInstruction_triggered();

    void on_actionClose_APP_triggered();

    void on_actionConverte_triggered();

    void on_actionOpen_App_triggered();

    void on_savetofile_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
