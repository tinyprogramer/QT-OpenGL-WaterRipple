#include "mainwindow.h"

#include "ui_mainwindow.h"

#include "dialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_ripplewidget(nullptr)
{
    ui->setupUi(this);
    this->resize(1024,728);

    this->setMouseTracking(true);

    //ui->centralWidget->setMouseTracking(true);
    //ui->splitter->setMouseTracking(true);
    //ui->splitter->widget(0)->installEventFilter(this);
    //m_ripplewidget=new RippleWidget(this);
    //m_ripplewidget->lower();

    ui->splitter->widget(0)->setMouseTracking(true);
    ui->splitter->widget(0)->resize(720,720);

    m_ripplewidget=new RippleWidget(ui->splitter->widget(0));//if you want the ripplewidget fullfill the mainwindow
    m_ripplewidget->stackUnder(ui->pushButton_2);//you should uncomment other lines above and comment this two lines

    m_ripplewidget->setRadius(20);
    m_ripplewidget->setStrength(0.01);
    m_ripplewidget->setResolution(2.0);
    m_ripplewidget->setDamping(0.995);

    m_ripplewidget->show();

    QObject::connect(ui->pushButton,&QPushButton::clicked,this,&MainWindow::showDialog);
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

void MainWindow::showDialog()
{
    Dialog d(this);
    d.exec();
}


