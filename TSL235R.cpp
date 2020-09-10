#include "TSL235R.h"

long long TSL235R::m_nCount=0;
long long TSL235R::m_nTime=0;
int TSL235R::m_nDataPin=0;

void TSL235R::irq()
{
    m_nCount++;
    if(m_nTime >= 100) detachInterrupt(digitalPinToInterrupt(m_nDataPin));
}

TSL235R::TSL235R(int pin)
  :
   m_fmW(0.f)
{
  m_nDataPin = pin;
}

void TSL235R::begin()
{
    pinMode(m_nDataPin, INPUT);
    digitalWrite(m_nDataPin, HIGH);
}

float TSL235R::readLightEnergy()
{
  m_nTime=0;
  attachInterrupt(m_nDataPin, irq, RISING);
  long long now = millis();
  while(m_nTime<=100) m_nTime = millis()-now;
  detachInterrupt(digitalPinToInterrupt(m_nDataPin));
  unsigned long hz = m_nCount - m_nLastCount;
  hz*=10;
  m_nLastCount = m_nCount;
  //Serial.print("FREQ: "); 
  //Serial.print(hz);
  //Serial.print("\t = ");
  m_fmW = hz/581.f;
  //Serial.print(m_fmW);  // +50 == rounding last digit
  //Serial.println(" mW/m2");
  return m_fmW;
}