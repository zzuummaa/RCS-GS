#include "telemetryloader.h"

#include <iostream>

using namespace std;

const int DATA_SIZE = 100;

telemetryLoader::telemetryLoader(redisDataService* dserv)
{
    this->dserv = dserv;
    this->lvFunc = NULL;
    this->pvFunc = NULL;
    this->type = 0;

    int cur_sec, cur_ms;
    getCurTime(&cur_sec, &cur_ms);
    this->exec_time_sec = cur_sec;
}

bool telemetryLoader::loadVal(long timeMS, int h, double *val) {
    if (lvFunc != NULL) {
        int time;
        return lvFunc(dserv, timeMS, h, &time, val);
    }

    string data;
    req_s req;
    req.type = type;
    req.ms = timeMS % 1000;
    req.sec = timeMS / 1000;

    if ( !dserv->get(&req, h, &data) ) {
        return false;
    }

    if (pvFunc == NULL) {
        cerr << "telemetryLoader: parseValueFunction is NULL" << endl;
        exit(1);
    }

    if ( !pvFunc(&data, val, req.ms + req.sec * 1000) ) {
        return false;
    }

    return true;
}

tel_pair telemetryLoader::loadVals(long start_time_ms, long end_time_ms, int step_ms) {
    QVector<double> timeVec;
    QVector<double> valVec;

    for (long i=start_time_ms; i < end_time_ms; i += step_ms) {
        double val;
        if ( loadVal(i, step_ms, &val) ) {
            timeVec.push_back(i - exec_time_sec);
            valVec.push_back(val);
        }
    }

    tel_pair tp(timeVec, valVec);
    return tp;
}

void telemetryLoader::setLoadValFunc(loadValFunc newlvFunc) {
    lvFunc = newlvFunc;
}

void telemetryLoader::setParseValFunc(parseValFunc newpvFunc) {
    pvFunc = newpvFunc;
}

tel_pair telemetryLoader::retreive()
{
    return loadVals(exec_time_sec * 1000l, (exec_time_sec + 100l) * 1000, 500);
}

redisDataService *telemetryLoader::getDataService()
{
    return dserv;
}

void telemetryLoader::setType(int type) {
    if (type < 0 && type > 255) {
        cerr << "telemetryLoader: invalid type = " << type;
        exit(1);
    }

    this->type = type;
}
