#include <Arduino.h>
#include "JC_Button.h"

#define LIGHT_MODE_PIN 2
#define SYNC_MODE_TIME 3100
#define CHANGE_COLOR_TIME 50

typedef enum
{
    Cntlr1,
    Cntlr2,
    NumControllers
}CntlrPins_Type;

typedef enum
{
     WAIT
    ,SYNC
}ContStates;

const uint8_t kubCntlrPins[] = {3,4};

Button LightMode(2);

void vSetSyncMode(uint8_t ubControllerPin);
void vChangeColorMode(uint8_t ubControllerPin);

void setup() 
{
    uint8_t ubCount = 0;

    for(ubCount = 0; ubCount < NumControllers; ubCount++)
    {
        pinMode(kubCntlrPins[ubCount], OUTPUT);
        digitalWrite(kubCntlrPins[ubCount], HIGH);
    }

    vSetSyncMode(kubCntlrPins[Cntlr2]);

    LightMode.begin();
}

void loop() 
{
    static ContStates eState = WAIT;

    /* Run the services */
    LightMode.read();

    switch (eState)
    {
        case WAIT:
            if(LightMode.wasReleased())
            {
                vChangeColorMode(kubCntlrPins[Cntlr1]);
            }
            else if (LightMode.pressedFor(1000))
            {
                eState = SYNC;
            }
            break;

        case SYNC:
            if(LightMode.wasReleased())
            {
                vSetSyncMode(kubCntlrPins[Cntlr2]);
                eState = WAIT;
            }  
            break; 
    }
    
}

void vSetSyncMode(uint8_t ubControllerPin)
{
    digitalWrite(ubControllerPin, LOW);

    delay(SYNC_MODE_TIME);

    digitalWrite(ubControllerPin, HIGH);
}

void vChangeColorMode(uint8_t ubControllerPin)
{
    digitalWrite(ubControllerPin, LOW);

    delay(CHANGE_COLOR_TIME);

    digitalWrite(ubControllerPin, HIGH);
}