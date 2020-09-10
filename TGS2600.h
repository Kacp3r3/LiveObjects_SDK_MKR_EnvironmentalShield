#pragma once

#include <Arduino.h>
class TGS2600
{
public:
    TGS2600(int enablePin, int analogPin);

public:
    void begin();
    float readPollutions();

private:
    int m_nAnalogPin;
    int m_nEnablePin;
};