#include "PMSA.h"

PMSA::PMSA(Uart& serial, int reset, int set)
    :
     m_Serial(serial)
    ,m_nPinReset(reset)
    ,m_nPinSet(set)
    ,values(data.values)
{
    memset(data.tab,0,32);
}

void PMSA::begin(int baud)
{
    m_Serial.begin(9600);
    pinMode(m_nPinSet,OUTPUT);
    digitalWrite(m_nPinSet,HIGH);
    pinMode(m_nPinReset,OUTPUT);
    digitalWrite(m_nPinReset,HIGH);
    uint8_t cmd[]={0x42,0x4d,0xe1,0x00,0x00,0x01,0x70};
    m_Serial.write(cmd,7);
}

void PMSA::printVals(Serial_& serial)
{
    serial.println();
    serial.println("---------------------------------------");
    serial.println("Concentration Units (standard)");
    serial.print("PM 1.0: "); serial.print(data.values.pm10_standard);
    serial.print("\t\tPM 2.5: "); serial.print(data.values.pm25_standard);
    serial.print("\t\tPM 10: "); serial.println(data.values.pm100_standard);
    serial.println("---------------------------------------");
    serial.println("Concentration Units (environmental)");
    serial.print("PM 1.0: "); serial.print(data.values.pm10_env);
    serial.print("\t\tPM 2.5: "); serial.print(data.values.pm25_env);
    serial.print("\t\tPM 10: "); serial.println(data.values.pm100_env);
    serial.println("---------------------------------------");
    serial.print("Particles > 0.3um / 0.1L air:"); serial.println(data.values.particles_03um);
    serial.print("Particles > 0.5um / 0.1L air:"); serial.println(data.values.particles_05um);
    serial.print("Particles > 1.0um / 0.1L air:"); serial.println(data.values.particles_10um);
    serial.print("Particles > 2.5um / 0.1L air:"); serial.println(data.values.particles_25um);
    serial.print("Particles > 5.0um / 0.1L air:"); serial.println(data.values.particles_50um);
    serial.print("Particles > 10.0 um / 0.1L air:"); serial.println(data.values.particles_100um);
    serial.println("---------------------------------------");
}
void PMSA::readValues()
{
    while(m_Serial.available()>0) m_Serial.read();
    uint8_t cmd[]= {0x42,0x4d,0xe2,0x00,0x00,0x01,0x71};
    m_Serial.write(cmd,7);
    char tab[32];
    while(m_Serial.available()< 32);
    if(m_Serial.peek()==0x42)
    {
        m_Serial.readBytes(tab, 32);

        int checkSum = (((tab[30])<<8 )|tab[31]); 
        int sum=0;
        for(int i=0;i<30;++i) sum+=tab[i];

        if(sum == checkSum)
        {
            for(int i=0;i<32;++i)
                data.tab[i]=tab[i];

            for(int i=0;i<32;i+=2)
            {
                char tmp = data.tab[i];
                data.tab[i] = data.tab[i+1];
                data.tab[i+1]= tmp;
            }
        }
    }
}