#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <qthread.h>
#include <QTimer>


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

       // sleep(20);
       // QTimer::singleShot(2500, splash,SLOT(close())); // Timer
       // QTimer::singleShot(2500,&w,SLOT(show()));
    splash.finish(&w);

    w.show();

    return a.exec();
}
