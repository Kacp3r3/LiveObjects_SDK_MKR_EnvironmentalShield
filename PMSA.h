#pragma once
#include <Arduino.h>
typedef uint8_t byte;

class PMSA
{
public:
    union _union
    {
    friend PMSA;
    private:
        char tab[32];
    public:
        struct _values{
        private:
        uint16_t framelen;
        public:
        uint16_t pm10_standard, pm25_standard, pm100_standard;
        uint16_t pm10_env, pm25_env, pm100_env;
        uint16_t particles_03um, particles_05um, particles_10um, particles_25um, particles_50um, particles_100um;
        private:
        uint16_t unused;
        uint16_t checksum;
        } values;
    }data;
public:
    PMSA(Uart& serial, int reset, int set);

public:
    void begin(int baud=9600);
    void printVals(Serial_& serial);
    void readValues();

public:
    _union::_values& values;

private:
    Uart& m_Serial;
    int m_nPinReset;
    int m_nPinSet;
};