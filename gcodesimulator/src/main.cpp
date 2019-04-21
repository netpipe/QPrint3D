#include "MainWindow.h"

#include <QtGui>
#include <QApplication>
#include "MainWindow.h"
#include "Helper.hpp"
#include <QWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    
    MainWindow w;
    w.show();
    if(argc==2)
    {
    	w.loadGCode(argv[1]);
    }
    
    return a.exec();
}
