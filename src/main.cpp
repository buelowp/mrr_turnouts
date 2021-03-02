#include <iostream>
#include <chrono>
#include <thread>
#include <wiringPi.h>

#include <QtCore/QCoreApplication>

#include "dispatch.h"
#include "messaging.h"

int main(int argc, char **argv) 
{
    QCoreApplication app(argc, argv);
    
    wiringPiSetupGpio();
    piHiPri(99);
    
    Messaging *messenger = new Messaging(QHostAddress("172.24.1.13"));
    Dispatch *dispatch = new Dispatch(messenger);
    messenger->connectToHost();
    
    qDebug() << __PRETTY_FUNCTION__ << ": Starting up";
    
    app.exec();
}
