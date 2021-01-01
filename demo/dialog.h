#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <ripplewidget.h>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    Ui::Dialog *ui;
    RippleWidget* m_rippleWidget;
};

#endif // DIALOG_H
