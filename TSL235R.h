#pragma once
#include <Arduino.h>

class TSL235R
{
public:
    TSL235R(int pin);

public:
    void begin();
    float readLightEnergy();
    
private:
    static void irq();

private:
    static int m_nDataPin;
    long long m_nLast = 0;
    static long long m_nCount;
    static long long m_nTime;
    long long m_nLastCount=0;
    float m_fmW;
};