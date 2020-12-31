#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _mywidget(0)
{
    ui->setupUi(this);
    this->resize(1024,728);
    this->setMouseTracking(true);
    ui->centralWidget->setMouseTracking(true);
    ui->splitter->setMouseTracking(true);
    ui->splitter->widget(0)->setMouseTracking(true);
    //ui->splitter->installEventFilter(this);
    //ui->centralWidget->installEventFilter(this);
    //ui->verticalLayout->installEventFilter(this);
    //qApp->installEventFilter(this);
    _mywidget=new MyWidget(ui->splitter->widget(0));
    _mywidget->stackUnder(ui->pushButton_2);
    //_mywidget->resize(this->size());
    qDebug()<<this->size();
    qDebug()<<ui->centralWidget->size();

    _mywidget->setRadius(20);
    _mywidget->setStrength(0.01);
    _mywidget->setResolution(2.0);
    _mywidget->setDamping(0.995);
    //_mywidget->setBackgroundImage("E:/github/ripplejs/jquery.ripples/demo/img/bg2.jpg");

    _mywidget->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::resizeEvent(QResizeEvent *event)
//{
//    //_mywidget->resize(this->size());
//}

//bool MainWindow::event(QEvent *event)
//{
//
//    if(event->type()==QEvent::MouseButtonPress||event->type()==QEvent::MouseMove)
//    {
//        _mywidget->accEvent(event);
//    }
//    return QMainWindow::event(event);
//}

//bool MainWindow::eventFilter(QObject *watched, QEvent *event)
//{
//
//    if(event->type()==QEvent::MouseButtonPress||event->type()==QEvent::MouseMove)
//    {
//        qDebug()<<watched->objectName();
//        qDebug()<<event->type();
//        qDebug()<<watched->parent();
//        //this->event(event);
//    }
//    return QMainWindow::eventFilter(watched,event);
//}


