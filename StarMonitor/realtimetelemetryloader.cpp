#include "realtimetelemetryloader.h"
#include <QLoggingCategory>

QLoggingCategory telloader_category("Telemetry loader");

realTimeTelemetryLoader::realTimeTelemetryLoader(redisDataService *dserv) : telemetryLoader(dserv)
{
    // Создание потока
    this->thread = new QThread;

    this->telp = new tel_pair();
}

realTimeTelemetryLoader::~realTimeTelemetryLoader()
{

}


void realTimeTelemetryLoader::start() {
    // Передаем права владения "рабочим" классом, классу QThread.
    this->moveToThread(thread);

    // Связываем сигнал об ошибки со слотом обработки ошибок(не показан).
    //connect(this, SIGNAL(error(QString)), this, SLOT(errorHandler(QString)));

    // Соединяем сигнал started потока, со слотом process "рабочего" класса, т.е. начинается выполнение нужной работы.
    connect(thread, SIGNAL(started()), this, SLOT(process()));

    // По завершению выходим из потока, и удаляем рабочий класс
    connect(this, SIGNAL(finished()), thread, SLOT(quit()));
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));

    // Удаляем поток, после выполнения операции
    connect(this, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}

void copyWithRemove(QVector<double> *dst, QVector<double> *src) {
    while (src->size() > 0) {
        dst->push_back(src->front());
        src->pop_front();
    }
}

tel_pair realTimeTelemetryLoader::retreive()
{
    mutex.lock();

    tel_pair tp;
    copyWithRemove(&(tp.first), &(telp->first));
    copyWithRemove(&(tp.second), &(telp->second));

    mutex.unlock();

    return tp;
}

void realTimeTelemetryLoader::setParseValFunc(parseValFunc newpvFunc)
{
    telemetryLoader::setParseValFunc(newpvFunc);
}

void realTimeTelemetryLoader::process()
{
    int cur_sec, cur_ms;
    int last_sec, last_ms;
    resp_s last_resp;

    getCurTime(&cur_sec, &cur_ms);
    while (1) {
        QThread::msleep(250);

        char buff[100];
        resp_s resp;
        resp.data = buff;

        if ( !dserv->getLast_(0, &resp) ) {
            continue;
        }

        if ( resp.type == last_resp.type &&
             resp.sec  == last_resp.sec  &&
             resp.ms   == last_resp.ms) {
            continue;
        }
        last_resp = resp;

        double val;

        if (pvFunc == NULL) {
            qDebug(telloader_category) << "ParseValueFunct is NULL";
            exit(1);
        }

        string tmp(resp.data, resp.dataSize);
        pvFunc(&tmp, &val, resp.ms + resp.sec * 1000);

        qDebug(telloader_category) << "New data is comming!";

        mutex.lock();

        static int offset = -1492390039;
        telp->first.push_back(resp.sec + offset);
        telp->second.push_back(val + offset);

        mutex.unlock();

    }
}
