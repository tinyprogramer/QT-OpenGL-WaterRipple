#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _mywidget(0)
{
    ui->setupUi(this);
    _mywidget=new MyWidget(this);
    ui->gridLayout->addWidget(_mywidget,0,0,1,1);
    _mywidget->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

