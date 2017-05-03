#include "telemetry.h"

using namespace std;

const string TITLE = "telemetry::";

void telemetry::addField(string fieldName, double val)
{
    if (telm->find(fieldName) != telm->end()) {
        cerr << TITLE << "addField: field '" << fieldName << "' already exists" << endl;
        exit(1);
    }

    (*telm)[fieldName] = val;
}

bool telemetry::fromStruct(int type, const char *tel_struct) {
    telm->clear();

    tel_camera* tc;
    tel_termo* tt;
    tel_barometer* tb;

    switch (telType) {
    case TYPE_CAMERA:
        tc = (tel_camera*)tel_struct;
        addField("photo_num", tc->photo_num);
        break;

    case TYPE_TERMO:
        tt = (tel_termo*)tel_struct;
        addField("sens_temp[0]", tt->sens_temp[0]);
        addField("sens_temp[1]", tt->sens_temp[1]);
        break;

    case TYPE_BAROMETER:
        tb = (tel_barometer*)tel_struct;
        addField("altitude", tb->altitude);
        addField("pressure", tb->pressure);
        break;

    default:
        return false;
    }

    return true;
}

telemetry::telemetry(int telType, char *tel_struct)
{
    telm = new map<string, double>();

    this->telType = telType;

    if (tel_struct == NULL) {
        char buff[64];
        fromStruct(telType, buff);
    } else {
        fromStruct(telType, tel_struct);
    }
}

telemetry::~telemetry()
{
    delete telm;
}

void telemetry::set(string fieldName, double val)
{
    if (telm->find(fieldName) == telm->end()) {
        cerr << TITLE << "set: Unknown fieldName '" << fieldName << "'" << endl;
        exit(1);
    }

    (*telm)[fieldName] = val;
}

double telemetry::get(string fieldName) {
    if (telm->find(fieldName) == telm->end()) {
        cerr << TITLE << "get: Unknown fieldName '" << fieldName << "'" << endl;
        exit(1);
    }

    return telm->find(fieldName)->second;
}
