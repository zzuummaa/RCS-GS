#include "telemetryparser.h"

bool telemetryParser::parse(string* data, double* outVal, int ms) {
    if ( !tel->fromStruct(tel->getTelType(), data->c_str()) ) {
        return false;
    }
    *outVal = tel->get(field);

    return true;
}

void telemetryParser::setField(string field)
{
    this->field = field;
}

telemetryParser::telemetryParser(int type)
{
    tel = new telemetry(type);
}

