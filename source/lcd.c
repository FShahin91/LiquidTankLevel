/*
 * File:   lcd.c
 * Author: Trion Projects
 * Modified by: Faris Shahin
 * LCD Library for 16x4 LCDs.
 * This library was modified to work with the
 * LCD model 1604A from Vatronix. Datasheet can
 * be found in this link (checked on 06/JAN/2022):
 * https://panda-bg.com/datasheet/2135-091859-LCD-module-TC1604A-02WA0-16x4-STN.pdf
 */

#include "config.h"
int8_t current_pos = 0;
// ---
void LCDInitialize() {
    // Set TRIS as output and clear PORT
    *LCD_TRIS_DATA = 0;
    *LCD_PORT_DATA = 0;
    *LCD_TRIS_CTRL = 0;
    *LCD_PORT_CTRL = 0;
    
    /*******************************************
     If needed clear analog pins which LCD uses
    *******************************************/
    
    // Wait for initialize
    __delay_ms(50);

    // Function Set
    // DL: 8-bit, N: 2-line, F: 5x8 dots
    LCDSendNibble(0x38);
    // Display ON/OFF control
    // D: Display ON, C: Cursor OFF, B: Cursor Blink OFF
    LCDSendNibble(lcdDisplayControl);
    LCDSendNibble(ClearDisplay);
    __delay_ms(3);
    // Entry Mode Set
    // I/D: Cursor/blink moves to right and DDRAM address is increased by 1
    // SH: Shifting entire display is performed
    LCDSendNibble(lcdEntryMode);
    __delay_ms(30);
}

// Clear display
void LCDClearDisplay(void) {
    LCDSendByte(0, ClearDisplay);
    current_pos = 0;
    __delay_ms(2);
}

// Return cursor to home
void LCDReturnHome(void) {
    LCDSendByte(0, ReturnHome);
    __delay_ms(2);
}

// Toggle display
/* 1 < time < 256
 * 1 < n < 256
 */
void LCDDisplayToggle(uint8_t time, uint8_t n) {
    // Blink LCD n times
    for(n; n > 0; n--) {
        LCDDisplayOff();
        for(uint8_t i = time; i > 0; i--) {
            __delay_ms(100);
        }
        LCDDisplayOn();
        for(uint8_t i = time; i > 0; i--) {
            __delay_ms(100);
        }
    }
}

// Display lcd on
void LCDDisplayOn(void) {
    lcdDisplayControl |= DisplayOn;
    LCDSendByte(0, lcdDisplayControl);
}

// Display lcd off
void LCDDisplayOff(void) {
    lcdDisplayControl &= ~DisplayOff;
    LCDSendByte(0, lcdDisplayControl);
}

// Cursor on
void LCDCursorOn(void) {
    lcdDisplayControl |= CursorOn;
    LCDSendByte(0, lcdDisplayControl);
}

// Cursor off
void LCDCursorOff(void) {
    lcdDisplayControl &= ~CursorOff;
    LCDSendByte(0, lcdDisplayControl);
}

// Cursor blink on
void LCDCursorBlinkOn(void) {
    lcdDisplayControl |= CursorBlinkOn;
    LCDSendByte(0, lcdDisplayControl);
}

// Cursor blink off
void LCDCursorBlinkOff(void) {
    lcdDisplayControl &= ~CursorBlinkOff;
    LCDSendByte(0, lcdDisplayControl);
}

// Shift display right
void LCDShiftDisplayRight(void) {
    lcdCursorDisplayShift |= ShiftDisplay;
    lcdCursorDisplayShift |= ShiftToRight;
    LCDSendByte(0, lcdCursorDisplayShift);
}

// Shift display left
void LCDShiftDisplayLeft(void) {
    lcdCursorDisplayShift |= ShiftDisplay;
    lcdCursorDisplayShift &= ~ShiftToLeft;
    LCDSendByte(0, lcdCursorDisplayShift);
}

// Shift cursor right
void LCDShiftCursorRight(void) {
    lcdCursorDisplayShift &= ~ShiftCursor;
    lcdCursorDisplayShift |= ShiftToRight;
    LCDSendByte(0, lcdCursorDisplayShift);
    current_pos++;
}

// Shift cursor left
void LCDShiftCursorLeft(void) {
    lcdCursorDisplayShift &= ~ShiftCursor;
    lcdCursorDisplayShift &= ~ShiftToLeft;
    LCDSendByte(0, lcdCursorDisplayShift);
    current_pos--;
}

// Send nibble to lcd
void LCDSendNibble(uint8_t nibble) {
    // Check nibbles bits and set PORT.
    // Commented part for use in 4-bit mode
    //*LCD_PORT_DATA = (unsigned)((nibble & 0b00000001) >> 0) ? (*LCD_PORT_DATA | (1 << LCD_D4)) : (*LCD_PORT_DATA & ~(1 << LCD_D4));
    //*LCD_PORT_DATA = (unsigned)((nibble & 0b00000010) >> 1) ? (*LCD_PORT_DATA | (1 << LCD_D5)) : (*LCD_PORT_DATA & ~(1 << LCD_D5));
    //*LCD_PORT_DATA = (unsigned)((nibble & 0b00000100) >> 2) ? (*LCD_PORT_DATA | (1 << LCD_D6)) : (*LCD_PORT_DATA & ~(1 << LCD_D6));
    //*LCD_PORT_DATA = (unsigned)((nibble & 0b00001000) >> 3) ? (*LCD_PORT_DATA | (1 << LCD_D7)) : (*LCD_PORT_DATA & ~(1 << LCD_D7));
    
    *LCD_PORT_DATA = nibble;
    // Send nibble to LCD.
    *LCD_PORT_CTRL |= 1 << LCD_EN;  // E pin - LCD Enable
    __delay_us(1);
    *LCD_PORT_CTRL &= ~(1 << LCD_EN);  // E pin - LCD Disable
    __delay_us(50); // min. 37us
}

// Send byte to lcd
void LCDSendByte(uint8_t reg, uint8_t byte) {
    *LCD_PORT_CTRL = reg ? (*LCD_PORT_CTRL | (1 << LCD_RS)) : (*LCD_PORT_CTRL & ~(1 << LCD_RS));  // RS pin - Register Select
    
    *LCD_PORT_CTRL &= ~(1 << LCD_RW);  // RW pin to write mode
    LCDSendNibble(byte);
    // Commented part for use in 4-bit mode (Faris Shahin)
    //LCDSendNibble(byte >> 4);
    //LCDSendNibble(byte & 0x0f);
}

/*
************** Display Position ***************
********** In Decimal based on shift **********
1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16
-----------------------------------------------
00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15
40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55
16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31
56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71

 Notes:
 * Position resets to first line on 80.
 * 32 to 39 is off screen. Same as 72 to 79.
 * (Faris Shahin)
 */

// Set position
void LCDSetPos(uint8_t x, uint8_t y) {
    int8_t addr = 0;
    uint8_t new_pos = 0;
    switch(y)
    {
        case 1:
            addr = 0;
            break;
        case 2:
            addr = 40;
            break;
        case 3:
            addr = 16;
            break;
        case 4:
            addr = 56;
            break;
        default:
            addr = 0;
            break;
    }
    
    addr += x-1;
    new_pos = addr;
    addr -= current_pos;
  //For some reason, the command to set the address in the
  //LCD memory isn't working properly. Hence, a shifitng method
  //is used to change the position as a workaround (Faris Shahin)
    if(addr > 0)
        for(int8_t i=0; i<addr; i++)
            LCDShiftCursorRight();
    else if(addr <0)
        for(int8_t i=0; i>addr; i--)
            LCDShiftCursorLeft();
    current_pos = new_pos;
}

// Print one character to LCD
void LCDPrintChar(uint8_t ch, uint8_t y, uint8_t x) {
    LCDSetPos(x, y);
    LCDSendByte(1, ch);
    current_pos++;
}

// Print string to LCD
void LCDPrintString(uint8_t *string, uint8_t y, uint8_t x) {
    // Set position
    // If x = 0, then print string to center
    if(x == 0) {
        for(uint8_t i = 0; i < 17; i++, x++) {
            if(string[i] == '\0') break;
        }
        LCDSetPos((18-x)/2, y);
    } else {
        LCDSetPos(x, y);
    }
    
    // Write each char
    for(uint8_t i = 0; i < 17; i++) {
        if(string[i] == '\0') break;
        LCDSendByte(1, string[i]);
        current_pos++;
    }
}
/*
 * Clears a line on the LCD and returns to the start of the line
 * Parameters:
 *      y - line number to clear
 */
void LCDClearLine(uint8_t y)
{
    LCDPrintString("                ", y, 1);
    LCDSetPos(1, y);    
}
