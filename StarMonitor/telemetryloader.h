#ifndef TELEMETRYLOADER_H
#define TELEMETRYLOADER_H

#include <qvector.h>
#include <shm/redis.h>

using namespace std;

typedef pair< QVector<double>, QVector<double> > tel_pair;
typedef bool (*loadValFunc)(redisDataService* dserv, long timeMS, int h, int* time, double* val);

class parseValClass {
public:
    virtual bool parse(string* data, double* outVal, int ms) {return false;}
};

class telemetryLoader
{
protected:
    redisDataService* dserv;
    loadValFunc lvFunc;
    parseValClass* pvClass;
    int type;
    int exec_time_sec;
public:
    telemetryLoader(redisDataService* dserv = 0);

    tel_pair loadVals(long start_time_ms, long end_time_ms, int step_ms);
    bool loadVal(long timeMS, int h, double* val);

    bool getKey(int time, int h, string* key);

    void setType(int type);

    void setLoadValFunc(loadValFunc newlvFunc);
    void setParseValClass(parseValClass *newpvClass);

    virtual tel_pair retreive();

    redisDataService* getDataService();
    int getExecTime() {return exec_time_sec;}
    void setExecTime(int new_exec_time_sec) {exec_time_sec = new_exec_time_sec;}
};

#endif // TELEMETRYLOADER_H
