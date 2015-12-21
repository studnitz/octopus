#include "client.h"
#include <QHostAddress>

Client::Client(QObject *parent) : QObject(parent)
{
    if(!this->connect(&client, SIGNAL(connected()),this, SLOT(startTransfer())))
    {
        qDebug() << "Could not start client";
    }
    else
    {
        qDebug() << "Client started";
    }


}


Client::~Client()
{
  client.close();
}

void Client::start(QString address, quint16 port)
{
  QHostAddress addr(address);
  client.connectToHost(addr, port);
}

void Client::startTransfer()
{
 client.write("Ready for Command\n", 18);
}


double Client::getCurrentValue(){
        double percent;
        FILE* file;
        unsigned long long totalUser, totalUserLow, totalSys, totalIdle, total;

        file = fopen("/proc/stat", "r");
        fscanf(file, "cpu %llu %llu %llu %llu", &totalUser, &totalUserLow,
            &totalSys, &totalIdle);
        fclose(file);


            total = (totalUser) + (totalUserLow) + (totalSys);
            percent = total;
            total += (totalIdle);
            percent /= total;
            percent *= 100;

        return percent;
    }

long Client::getAllMemory(){

    QFile file("/proc/meminfo");
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug()<<file.errorString();
    }

    QTextStream in(&file);
    QString line = in.readLine();


    QRegExp rx("[ ]");
    QStringList list = line.split(rx, QString::SkipEmptyParts);


    file.close();
    return atol(list.at(1).toStdString().c_str()  );
}

long Client::getFreeMemory(){
    QFile file("/proc/meminfo");
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug()<<file.errorString();
    }

    QTextStream in(&file);
    QString line = in.readLine();
    line = in.readLine();


    QRegExp rx("[ ]");
    QStringList list = line.split(rx, QString::SkipEmptyParts);


    file.close();
    return atol(list.at(1).toStdString().c_str()  );
}


//Funktion ist nur fuer die Statusabfrage vom server aus korrekt.
std::string Client::isConnected(){

    return "yes";
}
