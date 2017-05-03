#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <iostream>
#include <map>
#include <structs.h>

using namespace std;

class telemetry
{
private:
    map<string, double>* telm;

    friend void initializeTelemetry(telemetry* tel);
public:
    telemetry(int telType, char* tel_struct);
    ~telemetry();

    map<string, double>* asMap() {
        return telm;
    }

    double get(string fieldName);
    void set(string fieldName, double val);

    void addField(string fieldName, double val);
};

#endif // TELEMETRY_H
