#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <qthread.h>
#include "Util.h"
#include "FabAtHomePrinter.h"



namespace Ui
{
    class MainWindow;
}
















//A thread to handle printing.
class PrintThread : public QThread
{
    Q_OBJECT

protected:
    void run()
    {
    }
};


//A thread to handle GUI components.
class GUIComponentsThread : public QThread
{
    Q_OBJECT

protected:
    bool keepGoing;


public:
    GUIComponentsThread():keepGoing(false)
    {
    }

    void stop()
    {
        keepGoing = false;
    }

protected:
    void run()
    {
        while(keepGoing)
        {
        }
    }
};

















class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Ui::MainWindow* ui;

private:
    FabAtHomePrinter printer; //The printer.
    GUIComponentsThread gcThread;
    PrintThread pThread;

private slots:
    void on_loadConfigButton_clicked();
    void on_exitButton_clicked();
};






























#endif // MAINWINDOW_H
