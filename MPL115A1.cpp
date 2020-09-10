#include "MPL115A1.h"

MPL115A1::MPL115A1(int cs_pin)
    :
     m_nCSPIN(cs_pin)
    ,m_bInitialized(false)
{}

void MPL115A1::begin()
{
  SPI.begin();   

  pinMode(m_nCSPIN, OUTPUT);
  digitalWrite(m_nCSPIN, HIGH);
  delay (10);
  
  unsigned int A0H = readRegister(A0MSB);
  unsigned int A0L = readRegister(A0LSB);
  m_fA0 = (A0H << 5) + (A0L >> 3) + (A0L & 0x07) / 8.0;
  
  unsigned int B1H = readRegister(B1MSB);
  unsigned int B1L = readRegister(B1LSB);
  m_fB1 = ( ( ( (B1H & 0x1F) * 0x100)+B1L) / 8192.0) - 3 ;
  
  unsigned int B2H = readRegister(B2MSB);
  unsigned int B2L = readRegister(B2LSB);
  m_fB2 = ( ( ( (B2H - 0x80) << 8) + B2L) / 16384.0 ) - 2 ;
  
  unsigned int C12H = readRegister(C12MSB);
  unsigned int C12L = readRegister(C12LSB);
  m_fC12 = ( ( ( C12H * 0x100 ) + C12L) / 16777216.0 ) ;
  m_bInitialized=true;
}
float MPL115A1::readPressure()
{
  if(!m_bInitialized) return -1.f;
  digitalWrite(m_nCSPIN, LOW);
  delay(3);
  SPI.transfer(CONVERT);SPI.transfer(0x00);
  digitalWrite(m_nCSPIN, HIGH);
  delay(3);
  digitalWrite(m_nCSPIN, LOW);
  SPI.transfer(PRESH); unsigned int presH = SPI.transfer(0x00);
  delay(3);
  SPI.transfer(PRESL); unsigned int presL = SPI.transfer(0x00);
  delay(3);
  SPI.transfer(TEMPH); unsigned int tempH = SPI.transfer(0x00);
  delay(3);
  SPI.transfer(TEMPL); unsigned int tempL = SPI.transfer(0x00);
  delay(3);
  SPI.transfer(0x00);
  delay(3);
  digitalWrite(m_nCSPIN, HIGH);

  unsigned long press = ((presH *256) + presL)/64;
  unsigned long temp  = ((tempH *256) + tempL)/64;

  float pressure = m_fA0+(m_fB1+m_fC12*temp)*press+m_fB2*temp;
  float preskPa = pressure*  (65.0/1023.0)+50.0;

  return preskPa;
}

unsigned int MPL115A1::readRegister(byte thisRegister ) {
  digitalWrite(m_nCSPIN, LOW);
  delay(10);
  SPI.transfer(thisRegister);
  unsigned int result = SPI.transfer(0x00);
  digitalWrite(m_nCSPIN, HIGH);
  return result;
}