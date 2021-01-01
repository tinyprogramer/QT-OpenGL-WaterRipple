#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "ripplewidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void showDialog();

protected:
    virtual bool eventFilter(QObject *watched, QEvent *event);

private:
    Ui::MainWindow *ui;
    RippleWidget* m_ripplewidget;
};

#endif // MAINWINDOW_H
