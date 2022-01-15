/* 
 * File:   sm.h
 * Author: Faris Shahin
 * Comments:
 * This file along with the associated C file includes all the necessary 
 * configurations and functions to implement a state machine which is the main
 * powerhouse of the application.
 * 
 * Revision History: v1.0
 */

#ifndef SM_H
#define	SM_H

// Define the states
#define ST_IDLE         0
#define ST_VIEW         1
#define ST_OPTIONS      2
#define ST_ADD_EDIT     20
#define ST_DEL          21

//Define the events
#define EV_KEY_STAR     10
#define EV_KEY_HASH     11
#define EV_KEY_ONE      1
#define EV_KEY_TWO      2
#define EV_KEY_THREE    3
#define EV_KEY_FOUR     4
#define EV_KEY_NONE     254
#define EV_ANY          255

uint8_t TMR0of = 0;
uint8_t arrSize;

//Define a strucutre for the liquid tanks which includes the user name and the 
//tank's dimensions. The tanks are of cuboid shapes for this application
struct liquidTank{
    uint8_t name [7];
    uint16_t length;
    uint16_t width;
    uint16_t height;
}liquidTanks[4]; //In case the application is modified to use less or more sensors, do the necessary change to the size of this array

uint8_t init(void);
uint8_t idle (void);
uint8_t options(void);
uint8_t addEditEntry (void);
uint8_t deleteEntry(void);
uint8_t view(void);
uint8_t getEvent(void);

#endif	/* SM_H */
