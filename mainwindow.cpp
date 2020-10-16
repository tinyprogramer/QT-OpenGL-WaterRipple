#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _mywidget(0)
{
    ui->setupUi(this);
    //this->resize(QSize(1024,1024));
    _mywidget=new MyWidget(this);
    ui->gridLayout->addWidget(_mywidget,0,0,1,1);
    //ui->gridLayout->itemAtPosition(0,0)->setGeometry(QRect(0,0,1024,1024));
    //_mywidget->stackUnder(ui->widget);
    _mywidget->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

