#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"

#include <QDesktopWidget>
#include <QDateTime>
#include "qdebug.h"

#include "src/fasttable/fasttablewidget.h"
#include "src/main/testdialog.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_testButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
