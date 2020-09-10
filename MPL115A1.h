#pragma once
#include <SPI.h>

#define PRESH	0x80
#define	PRESL	0x82
#define	TEMPH	0x84
#define	TEMPL	0x86

#define A0MSB	0x88
#define A0LSB	0x8A
#define B1MSB	0x8C
#define B1LSB	0x8E
#define	B2MSB	0x90
#define B2LSB	0x92
#define C12MSB	0x94
#define	C12LSB	0x96

#define CONVERT	0x24	

class MPL115A1
{
public:
    MPL115A1(int cs_pin);

    void begin();
    float readPressure();

private:
    unsigned int readRegister(byte thisRegister);
private:
    int m_nCSPIN;
    float m_fA0;
    float m_fB1;
    float m_fB2;
    float m_fC12;
    bool m_bInitialized;
};