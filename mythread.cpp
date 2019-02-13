#include "mythread.h"

MyThread::MyThread()
{
}

void MyThread::run()
{
    for (int i = 0; i < 10; ++i)
    {
        qDebug() << this->name << " " << i;
        this->sleep(1);
    }
}
