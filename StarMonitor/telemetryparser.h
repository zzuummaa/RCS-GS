#ifndef TELEMETRYPARSER_H
#define TELEMETRYPARSER_H

#include "telemetry.h"
#include "telemetryloader.h"

class telemetryParser : public parseValClass
{
private:
    string field;

public:
    telemetry* tel;

    telemetryParser(int type);

    bool parse(string* data, double* outVal, int ms);

    void setField(string field);
};

#endif // TELEMETRYPARSER_H
