#include "mainwindow.h"
#include <QApplication>
#include "mythread.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


//    MyThread t1;
//    t1.name = "t1#";
//    t1.start();

//    MyThread t2;
//    t2.name = "t2#";
//    t2.start();






    return a.exec();
}
