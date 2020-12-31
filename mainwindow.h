#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "mywidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    //virtual void resizeEvent(QResizeEvent* event);
    //virtual bool event(QEvent *event);
    //virtual bool eventFilter(QObject *watched, QEvent *event);

private:
    Ui::MainWindow *ui;
    MyWidget* _mywidget;
};

#endif // MAINWINDOW_H
