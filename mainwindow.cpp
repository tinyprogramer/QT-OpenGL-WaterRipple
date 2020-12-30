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
    _mywidget->setRadius(20);
    _mywidget->setStrength(0.01);
    _mywidget->setResolution(2.0);
    _mywidget->setDamping(0.995);
    //_mywidget->setBackgroundImage("E:/github/ripplejs/jquery.ripples/demo/img/bg2.jpg");
    this->resize(1024,728);
    _mywidget->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

