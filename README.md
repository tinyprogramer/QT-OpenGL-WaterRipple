After a period of coding,I think this program meet my expectation eventually.

This program references a javascript code,you can even use it with QWebEngine in QT to get this effect.[https://github.com/sirxemic/jquery.ripples](https://github.com/sirxemic/jquery.ripples "Water Ripple")

But I want to make it more convinience with QT.

![image](https://github.com/tinyprogramer/QT-OpenGL-WaterRipple/blob/master/demo/img/ripple.gif)

If you want to have a danymic ripple background under a QWidget, you can code in your widget class as follows.

    Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),m_rippleWidget(nullptr)
    {
    	ui->setupUi(this);
    
    	m_rippleWidget=new RippleWidget(this);
    	m_rippleWidget->lower();

		m_ripplewidget->setRadius(20);
    	m_ripplewidget->setStrength(0.01);
    	m_ripplewidget->setResolution(2.0);
    	m_ripplewidget->setDamping(0.995);
    	m_rippleWidget->setBackgroundImage("xxxx");
    }

All parameter except backgroundImage have an avliable default value,you don't have to set them,but you can adjust them to get a satisfactory effect as you wish.

Note that the setResolution may make you confused because with larger number,it has lower quality....

You can also reference my demo,it's shows one possible way to get it work correctly under mainwindow and dialog.

However,if you have some special need for the effect,you may have to overwrite some functions about **QEvent** ,or rewrite my codes,the key is let RippleWidget get correct MouseEvent. 

For more information you could go to my [CSDN](https://blog.csdn.net/qq_41961619/article/details/109171428) blog.



