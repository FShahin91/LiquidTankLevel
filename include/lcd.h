/* 
 * File:   lcd.h
 * Author: Trion Projects
 * Comments:
 * Revision history: v1.0
 */

#ifndef LCD_H
#define LCD_H

// *****************************************************************************
// ************************** Edit Before Use Library **************************
// Write which PORT and TRIS is LCD uses.
volatile uint8_t *LCD_PORT_DATA = &PORTB;
volatile uint8_t *LCD_TRIS_DATA = &TRISB;

volatile uint8_t *LCD_PORT_CTRL = &PORTD;
volatile uint8_t *LCD_TRIS_CTRL = &TRISD;

// Adjust pins in define section.
#define LCD_RS  5     // The RS bit of the PORT_CTRL
#define LCD_RW  6     // The RW bit of the PORT_CTRL
#define LCD_EN  7     // The EN bit of the PORT_CTRL
#define LCD_D0  0     // The D0 bit of the PORT_DATA
#define LCD_D1  1     // The D1 bit of the PORT_DATA
#define LCD_D2  2     // The D2 bit of the PORT_DATA
#define LCD_D3  3     // The D3 bit of the PORT_DATA
#define LCD_D4  4     // The D4 bit of the PORT_DATA
#define LCD_D5  5     // The D5 bit of the PORT_DATA
#define LCD_D6  6     // The D6 bit of the PORT_DATA
#define LCD_D7  7     // The D7 bit of the PORT_DATA

// *****************************************************************************

#define ClearDisplay        0b00000001
#define ReturnHome          0b00000010
#define DisplayOn           0b00000100
#define DisplayOff          0b00000100
#define CursorOn            0b00000010
#define CursorOff           0b00000010
#define CursorBlinkOn       0b00000001
#define CursorBlinkOff      0b00000001
#define ShiftToLeft         0b00000100
#define ShiftToRight        0b00000100
#define ShiftCursor         0b00001000
#define ShiftDisplay        0b00001000
#define FirstLine           0b10000000
#define SecondLine          0b11000000

uint8_t lcdEntryMode = 0b00000110;
uint8_t lcdDisplayControl = 0b00001100;
uint8_t lcdCursorDisplayShift = 0b00010000;

void LCDInitialize();
void LCDSendByte(uint8_t reg, uint8_t byte);
void LCDSendNibble(uint8_t nibble);
void LCDCommand(uint8_t byte);
void LCDClearDisplay(void);
void LCDReturnHome(void);
void LCDDisplayToggle(uint8_t time, uint8_t n);
void LCDDisplayOn(void);
void LCDDisplayOff(void);
void LCDCursorOn(void);
void LCDCursorOff(void);
void LCDCursorBlinkOn(void);
void LCDCursorBlinkOff(void);
void LCDShiftDisplayRight(void);
void LCDShiftDisplayLeft(void);
void LCDShiftCursorRight(void);
void LCDShiftCursorLeft(void);
void LCDSetPos(uint8_t x, uint8_t y);
void LCDPrintChar(uint8_t ch, uint8_t y, uint8_t x);
void LCDPrintString(uint8_t *string, uint8_t y, uint8_t x);
void LCDClearLine(uint8_t y);

#endif
