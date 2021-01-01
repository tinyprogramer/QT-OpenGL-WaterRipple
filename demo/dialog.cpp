#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),m_rippleWidget(nullptr)
{
    ui->setupUi(this);

    m_rippleWidget=new RippleWidget(this);
    m_rippleWidget->lower();
    m_rippleWidget->setBackgroundImage(":/img/bg2.jpg");
}

Dialog::~Dialog()
{
    if(m_rippleWidget)
    {
        delete m_rippleWidget;
    }
    delete ui;
}
