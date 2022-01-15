/*
 * File:   main.c
 * Author: Faris Shahin
 *
 * Revision: v1.0
 */

#include "config.h"

// define a structure for the state machine
typedef struct {
    uint8_t ST;             //Represents the current state
    uint8_t EV;             //Represents the event that occurred
    uint8_t (*FN)(void);    //Represents the function which will be called
} smTransition;

void main(void)
{
    //Initialize MCU registers which will be used
    ADCON1 = 0x06;      //Set PORTA as digital for use with ultrasonic module
    OPTION_REG = 0x85;  //PORTB pull ups disabled, TMR0 internal clk, 64 prescalar
    INTCON = 0x80;      //Set Global Interrupt Enable bit
    TRISA = 0x00;       //Set PORTA as output
    CCP1CON = 0x00;     //Disable Capture/Compare/PWM
    RCSTA = 0x00;       //Disable serial port and all associated functions of serial communication
    
    //Initialize all modules
    LCDInitialize();
    UltraSonicInit();
    KeypadInit();
    
    //Set the behavior of the state machine
    smTransition transitions[] = {
    {ST_IDLE, EV_KEY_NONE, &idle},
    {ST_IDLE, EV_KEY_STAR, &options},
    {ST_OPTIONS, EV_KEY_ONE, &addEditEntry},
    {ST_OPTIONS, EV_KEY_TWO, &deleteEntry},
    {ST_OPTIONS, EV_KEY_THREE, &view},
    {ST_VIEW, EV_KEY_NONE, &idle},
    {ST_ADD_EDIT, EV_KEY_HASH, &options},
    {ST_DEL, EV_KEY_HASH, &options}
    };
    
    uint8_t stCount = sizeof(transitions)/sizeof(*transitions);
    
    //initialize the state machine and set the event to "any"
    uint8_t currentState = init();
    uint8_t event = EV_ANY;
    
    while(1)
    {
        //Infinite loop to go through the states in the state machine
        event = getEvent();
        for(uint8_t i = 0; i < stCount; i++)
            if(currentState == transitions[i].ST)
            {
                if(event == transitions[i].EV)
                {
                    currentState = (transitions[i].FN)();
                    break;
                }
            }
    }
}

/*
 * Interrupt service routine. Only checks TMR0 interrupt flag and increments a 
 * counter if the flag is set (sets only when TMR0 overflows)
 */
void __interrupt() tc_int(void)
{
    if (TMR0IF)
    {
        TMR0of++;
        TMR0IF = 0;
    }
}