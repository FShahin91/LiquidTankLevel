/*
 * File:   KeyPad.c
 * Author: Faris Shahin
 *
 * Library for 4x3 keypad
 */


#include "config.h"

/*
 *  Initiates the pins connected to the keypad.
 *  Pin numbers are selected in the header file.
 *  All rows are configured as inputs. All columns are configured as output.
 */
void KeypadInit(void)
{
    *OUT_TRIS &= ~(1 << (COL1-1));
    *OUT_TRIS &= ~(1 << (COL2-1));
    *OUT_TRIS &= ~(1 << (COL3-1));
    *IN_TRIS |= 1 << (ROW1-1);
    *IN_TRIS |= 1 << (ROW2-1);
    *IN_TRIS |= 1 << (ROW3-1);
    *IN_TRIS |= 1 << (ROW4-1);
}

/*
 *  Read the pressed button from the keypad
 *  Returns:
 *      The pressed button
 */
uint8_t KeypadRead(void)
{
    static uint8_t rows[4] = {ROW1, ROW2, ROW3, ROW4};
    static uint8_t col[3] = {COL1, COL2, COL3};
    
    /* Use a "moving 1" method to determine the pressed key:
     * At the start of each loop, a 1 is sent from a column.
     * Iterate through all rows and see if you read this 1
     * If a 1 is read, return the keypad[row][column] combination
     * If nothing is read through this process, return 0
    */
    for(uint8_t i=0; i<3; i++)
    {
        *OUT_KEYS = 0x00;  //Reset OUT_KEYS
        *OUT_KEYS = 1 << (col[i]-1);  //Send a 1 to the current indexed column
        for(uint8_t j=0; j<4; j++)    //Loop through all rows
            if((*IN_KEYS >> (rows[j]-1)) & 0x01)    //Check if 1 is read
            {
                __delay_ms(350);        //Delay to cancel multiple continuous reads of the button press
                return Keypad[j][i];    //Return the pressed button
            }        
    }
    return 0;   //Return 0 if no button is pressed
}
