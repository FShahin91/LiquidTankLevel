/*
 * File:   ultrasonic_hcsr04.c
 * Author: Faris
 * 
 * Library for HC-SR04 ultrasonic
 * 
 * Note: This library relies on timers to properly work. Make sure you have
 * configured the timer you wish to use correctly in main.c and do the
 * necessary changes in this library.
 */

#include "config.h"

/*
 * Initiates the pins connected to the ultrasonic.
 * Pin numbers are selected in the header file.
 * Echo pin is configured as input. Trigger pin is configured as output.
 */
void UltraSonicInit()
{
    //PIN indexing in PIC microcontroller is zero-based
   TRIG_PIN--;
   ECHO_PIN--;
   *US_TRIS &= ~(1<<TRIG_PIN);  //Set the trigger as output
   *US_TRIS |= 1<< ECHO_PIN;    //Set the echo as input
}

/*
 *  Function that takes the ultrasonic reading
 *  Parameters:
 *      *TMRCount: A pointer to a timer overflow counter
 *  Returns:
 *      Distance in centimeters measured by the ultrasonic
 */
uint16_t UltraSonicPing(uint8_t * TMRCount)
{
    uint16_t tmp = 0;
    *TMRCount = 1;              //Reset the timer overflow counter back to 1
    *US_DATA |= 1<<TRIG_PIN;    //Send the trigger signal
    __delay_us(10);             //Wait for 10us
    *US_DATA &= ~(1<<TRIG_PIN); //Reset the trigger signal. This concludes the trigger sequence
    
    TMR0IE = 1;                 //Enable TMR0 interrupt. If a different timer is used, do the necessary modification here
    TMR0 = 0;                   //Set TMR0 to 0
    
    //Wait for the echo signal
    while(!((*US_DATA>>ECHO_PIN)&0x01) && *TMRCount < 25);
    //If too much time passed without echo, return 0 to distance
    if(*TMRCount >= 25)
    {
        return 0;
    }
    TMR0 = 0;
    *TMRCount = 1;
    //Determine the echo signal time (high-input signal time)
    while((*US_DATA>>ECHO_PIN)&0x01 && *TMRCount < 25);
    tmp = TMR0;
    TMR0IE = 0; //Disable TMR) interrupt
    
    //If too much time passed while still processing, return 0 to distance
    if(*TMRCount >= 25)
    {
        return 0;
    }
    //Return the calculated distance
    // Distance = (TMR0 * overflow times) * (1/internal clock) * prescaler * speed of sound in air
    // Speed of sound in air = 3400cm/s
    // In this project, the used crystal has a frequency of 4MHz -> Internal clock = 4MHz/4 (check PIC datasheet)
    // prescalar is set to 1:64
    return tmp * (*TMRCount) * 1.152;
}