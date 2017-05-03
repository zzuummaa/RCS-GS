#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <iostream>
#include <map>
#include <structs.h>

using namespace std;

class telemetry
{
private:
    int telType;

    map<string, double>* telm;

    friend void initializeTelemetry(telemetry* tel);
public:
    telemetry(int telType, char* tel_struct = NULL);
    ~telemetry();

    map<string, double>* asMap() {
        return telm;
    }

    double get(string fieldName);
    void set(string fieldName, double val);

    void addField(string fieldName, double val);

    bool fromStruct(int type, const char *tel_struct);

    int getTelType() {return telType;}
};

#endif // TELEMETRY_H
