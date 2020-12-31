#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _ripplewidget(0)
{
    ui->setupUi(this);
    this->resize(1024,728);

    this->setMouseTracking(true);

    //ui->centralWidget->setMouseTracking(true);
    //ui->splitter->setMouseTracking(true);
    //ui->splitter->widget(0)->installEventFilter(this);
    //_ripplewidget=new RippleWidget(this);
    //_ripplewidget->stackUnder(ui->centralWidget);

    ui->splitter->widget(0)->setMouseTracking(true);
    ui->splitter->widget(0)->resize(768,768);

    _ripplewidget=new RippleWidget(ui->splitter->widget(0));//
    _ripplewidget->stackUnder(ui->pushButton_2);//

    _ripplewidget->setRadius(20);
    _ripplewidget->setStrength(0.01);
    _ripplewidget->setResolution(2.0);
    _ripplewidget->setDamping(0.995);

    _ripplewidget->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{

    if(event->type()==QEvent::MouseButtonPress||event->type()==QEvent::MouseMove)
    {
        return false;
    }
    return QMainWindow::eventFilter(watched,event);
}


