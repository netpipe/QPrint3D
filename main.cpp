#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <qthread.h>
#include <QTimer>
#include "gsim/Helper.hpp"
#include <QDebug>

QStringList args;

class I : public QThread
{
public:
    static void sleep(unsigned long secs) { QThread::sleep(secs); }
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap pixmap("./media/QPrint3D.png");
    QSplashScreen splash(pixmap);
    a.setWindowIcon(QIcon(pixmap));
    splash.show();

   //     I::sleep(15); // splash is shown for 5 seconds
    MainWindow w;

    qInfo() << "checkikng args";
    QStringList args = a.arguments();
    if (args.count() != 2)
    {
        qInfo() << args.value(1);

        std::cerr << "argument required" << endl;
     //     return 1;
    }
    else{ //only one param
      //  qInfo() << args.value(1);
            w.passargs(args.value(1));
    }


       // sleep(20);
       // QTimer::singleShot(2500, splash,SLOT(close())); // Timer
       // QTimer::singleShot(2500,&w,SLOT(show()));
    splash.finish(&w);

    w.show();

    return a.exec();
}
