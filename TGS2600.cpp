#include "TGS2600.h"

TGS2600::TGS2600(int enablePin, int analogPin)
    :
     m_nEnablePin(enablePin)
    ,m_nAnalogPin(analogPin)
{}

void TGS2600::begin()
{
    analogReadResolution(12);
    pinMode(m_nEnablePin, OUTPUT);
    digitalWrite(m_nEnablePin, HIGH);
}
float TGS2600::readPollutions()
{
    return map(analogRead(m_nAnalogPin), 410, 2048,0.f,1.f);
}