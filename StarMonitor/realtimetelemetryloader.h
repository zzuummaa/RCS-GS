#ifndef REALTIMETELEMETRYLOADER_H
#define REALTIMETELEMETRYLOADER_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include "telemetryloader.h"

class realTimeTelemetryLoader : public QObject, telemetryLoader
{
    Q_OBJECT
public:
    realTimeTelemetryLoader(redisDataService* dserv);
    virtual ~realTimeTelemetryLoader();

    void start();
    tel_pair retreive();

    void setParseValFunc(parseValFunc newpvFunc);

    void setType(int type) {telemetryLoader::setType(type);}
    int getExecTime() {return telemetryLoader::getExecTime();}
    void setExecTime(int new_exec_time_sec) {telemetryLoader::setExecTime(new_exec_time_sec);}
public slots:
    void process();

signals:
    void finished();
    void error(QString err);

private:
    QThread* thread;
    tel_pair* telp;
    QMutex mutex;
};

void copyWithRemove(QVector<double> *dst, QVector<double> *src);

#endif // REALTIMETELEMETRYLOADER_H
