#include "radio.h"



Radio::Radio() :
    mRadio(RF_CEPIN, RF_CSPIN)
{
}


Radio::~Radio()
{
}

void Radio::Setup(Print& print)
{
    print.print(F("RADIO_SETUP..."));
    mRadio.begin();
    mRadio.setPALevel(RF24_PA_MAX);
    mRadio.setAutoAck(1);
    mRadio.setRetries(15, 15);
    mRadio.openReadingPipe(0, RFAddress[0]);
    mRadio.startListening();
    // wait until initialized
    delay(1000);
    print.println(F("COMPLETE"));

    //mRadio.printDetails();
}

void Radio::Read(void * buf, uint8_t len)
{
    mRadio.read(buf, len);
}

bool Radio::Available()
{
    return mRadio.available();
}
