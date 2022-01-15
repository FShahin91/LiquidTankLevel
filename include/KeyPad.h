/* 
 * File:   KeyPad.h
 * Author: Faris Shahin
 *
 * Library for 4x3 keypad
 * Revision History: v1.0
 */

#ifndef KEYPAD_H
#define	KEYPAD_H

// Set ports for the input pins and the output pins
volatile uint8_t * IN_TRIS = &TRISC;
volatile uint8_t * OUT_TRIS = &TRISC;
volatile uint8_t * IN_KEYS = &PORTC;
volatile uint8_t * OUT_KEYS = &PORTC;

//The rows and column pins of the keypad are organized as follows (left to right):
// ROW2, ROW3, COL3, ROW4, COL1, ROW1, COL2

#define ROW2 1  //Pin number for ROW2 in the keypad
#define ROW3 2  //Pin number for ROW3 in the keypad
#define COL3 3  //Pin number for COL3 in the keypad
#define ROW4 4  //Pin number for ROW4 in the keypad
#define COL1 5  //Pin number for COL1 in the keypad
#define ROW1 6  //Pin number for ROW1 in the keypad
#define COL2 7  //Pin number for COL2 in the keypad

uint8_t Keypad[4][3] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
    };

void KeypadInit(void);
uint8_t KeypadRead(void);

#endif	/* KEYPAD_H */

