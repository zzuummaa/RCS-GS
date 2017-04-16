#ifndef TELEMETRYLOADER_H
#define TELEMETRYLOADER_H

#include <qvector.h>
#include <shm/redis.h>

using namespace std;

typedef pair< QVector<double>, QVector<double> > tel_pair;
typedef bool (*loadValFunc)(redisDataService* dserv, int timeMS, int h, int* time, double* val);
typedef bool (*parseValFunc)(string* data, double* outVal);

class telemetryLoader
{
private:
    redisDataService* dserv;
    loadValFunc lvFunc;
    parseValFunc pvFunc;
    int type;
public:
    telemetryLoader(redisDataService* dserv);

    tel_pair loadVals(int start_time_ms, int end_time_ms, int step);
    bool loadVal(int timeMS, int h, double* val);

    bool getKey(int time, int h, string* key);

    void setType(int type);

    void setLoadValFunc(loadValFunc newlvFunc);
    void setParseValFunc(parseValFunc newpvFunc);
};

#endif // TELEMETRYLOADER_H
