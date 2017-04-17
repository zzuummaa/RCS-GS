#include "telemetryloader.h"

#include <iostream>

using namespace std;

const int DATA_SIZE = 100;

telemetryLoader::telemetryLoader(redisDataService* dserv)
{
    this->dserv = dserv;
    this->lvFunc = NULL;
    this->type = 0;
}

bool telemetryLoader::loadVal(int timeMS, int h, double *val) {
    if (lvFunc != NULL) {
        int time;
        return lvFunc(dserv, timeMS, h, &time, val);
    }

    string data;
    req_s req;
    req.type = 0;
    req.ms = timeMS % 1000;
    req.sec = timeMS / 1000;

    if ( !dserv->get(&req, h, &data) ) {
        return false;
    }

    if (pvFunc == NULL) {
        cerr << "telemetryLoader: parseValueFunction is NULL";
        exit(0);
    }

    if ( !pvFunc(&data, val, req.ms + req.sec * 1000) ) {
        return false;
    }

    return true;
}

tel_pair telemetryLoader::loadVals(int start_time_ms, int end_time_ms, int step_ms) {
    QVector<double> timeVec;
    QVector<double> valVec;

    for (int i=start_time_ms; i < end_time_ms; i += step_ms) {
        double val;
        if ( loadVal(i, step_ms, &val) ) {
            timeVec.push_back(i);
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

void telemetryLoader::setType(int type) {
    if (type < 0 && type > 255) {
        cerr << "telemetryLoader: invalid type = " << type;
        exit(1);
    }

    this->type = type;
}
