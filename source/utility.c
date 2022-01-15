#include "config.h"

/*
 * Convert numbers into characters
 * Parameters:
 *      num - the number to convert
 * Returns:
 *      A pointer to the first element of characters array representing the number
 * Note: Large liquid tanks for houses and apartments can't exceed 4 digits
 *  For that, a string of 4 characters should be enough to indicate a tank's
 *  liquid level
 */
uint8_t* NumToStr (uint32_t num)
{
    //Initialize the string array
    static uint8_t str[5] = {'\0','\0','\0','\0','\0'}; 
    uint8_t count = 4;
    //Iterate through the number and fill the array from right to left
    for(count; count>=0; count--)
    {
        str[count-1] = (num % 10) + 0x30; //ASCII code for numbers start from 0x30 for 0 to 0x39 for 9
        num /=10;
        if(num == 0)  //Check if the number was fully iterated
            break;
    }
    //Shift the numbers to start from the first location of the array: str[0]
    for(count; count>1; count--)
    {
        for(uint8_t i = 0; i<=3; i++)
        {
            str[i] = str[i+1];
        }
    }
    return str;
}

/*
 * Sets the name of the user in the system
 * Parameters:
 *      *arrName: A pointer to the name array where data will be stored
 *      arrSize: Size of the name array
 *      LCDLine: The line on the LCD screen where the name will be shown
 * Returns:
 *      An unsigned number to indicate either success or error:
 *          1: operation successful
 *          2: the user has set the first character as space {' '}
 * Notes:
 * In nameSet, the keypad will work as a directions key.
 * Number 2 is the up arrow, number 8 is the down arrow.
 * Number 4 is the left arrow, number 6 is the right arrow.
 * 
 * When pressing 2, the character will change to the next one as per the 
 * English language alphabetical order. When pressing 8, the opposite happens.
 * 
 * When pressing 4, the program will go to the previous character in the array
 *  down to the first character.
 * When pressing 6, the program will go to the next character in the array
 *  up to the last character specified in arrSize.
 * 
 * The characters will navigate as follows:
 * Space <-> capital letters <-> small letters <-> numbers <-> space 
 * 
 * No special characters are included here except space.
 * 
 * When pressing *, the program will return to the caller  
 */
uint8_t nameSet(uint8_t * arrName, uint8_t arrSize, uint8_t LCDline)
{
    uint8_t keyPush = 0; 
    uint8_t currentLoc = 0;
    while(keyPush != '*')
    {
        keyPush = 0;
        keyPush = KeypadRead();
        switch (keyPush)
        {
            case '2':
                if(arrName[currentLoc] == ' ')
                {
                    arrName[currentLoc] = 'A';
                    LCDPrintChar(arrName[currentLoc], LCDline, currentLoc+1);
                    LCDShiftCursorLeft(); //Return the cursor to the current location
                }
                else if(arrName[currentLoc] == 'Z')
                {
                    arrName[currentLoc] = 'a';
                    LCDPrintChar(arrName[currentLoc],LCDline, currentLoc+1);
                    LCDShiftCursorLeft();
                }
                else if(arrName[currentLoc] == 'z')
                {
                    arrName[currentLoc] = '0';
                    LCDPrintChar(arrName[currentLoc],LCDline, currentLoc+1);
                    LCDShiftCursorLeft();
                }
                else if(arrName[currentLoc] == '9')
                {
                    arrName[currentLoc] = ' ';
                    LCDPrintChar(arrName[currentLoc],LCDline, currentLoc+1);
                    LCDShiftCursorLeft();
                }
                else
                {
                    arrName[currentLoc] = ++arrName[currentLoc];
                    LCDPrintChar(arrName[currentLoc],LCDline, currentLoc+1);
                    LCDShiftCursorLeft();
                }
                break;
            case '8':
                if(arrName[currentLoc] == ' ')
                {
                    arrName[currentLoc] = '9';
                    LCDPrintChar(arrName[currentLoc], LCDline, currentLoc+1);
                    LCDShiftCursorLeft();
                }
                else if(arrName[currentLoc] == '0')
                {
                    arrName[currentLoc] = 'z';
                    LCDPrintChar(arrName[currentLoc], LCDline, currentLoc+1);
                    LCDShiftCursorLeft();
                }
                else if (arrName[currentLoc] == 'a')
                {
                    arrName[currentLoc] = 'Z';
                    LCDPrintChar(arrName[currentLoc], LCDline, currentLoc+1);
                    LCDShiftCursorLeft();
                }
                else if(arrName[currentLoc] == 'A')
                {
                    arrName[currentLoc] = ' ';
                    LCDPrintChar(arrName[currentLoc], LCDline, currentLoc+1);
                    LCDShiftCursorLeft();
                }
                else
                {
                    arrName[currentLoc] = --arrName[currentLoc];
                    LCDPrintChar(arrName[currentLoc], LCDline, currentLoc+1);
                    LCDShiftCursorLeft();
                }
                break;
            case '4':
                if(currentLoc > 0)
                {
                    LCDShiftCursorLeft();
                    currentLoc--;
                }
                break;
            case '6':
                if(currentLoc < arrSize)
                {
                    LCDShiftCursorRight();
                    currentLoc++;
                }
                break;
            default:
                break;
        }
    }
    if(arrName[0] == ' ')
        return 2;
    else
        return 1;
}

/*
 *  A function to set the value of width/length/height of liquid tanks as
 *  entered from the user.
 *  Parameters:
 *      LCDLine - The line on LCD where the data will be entered
 *  Returns:
 *      An unsigned integer representing the value
 *  Notes:
 *  The conversion from characters to an integer is basically the opposite 
 *  process of NumToStr function.
 *  The width, length and height is less than 1000 cm for this system.
 *  When pressing #, the value is reset to 0.
 *  When pressing *, the program will return to the caller.
 */
uint16_t numSet(uint8_t LCDline)
{
    uint8_t keyPush = 0;
    uint16_t num = 0, currentLoc = 0;
    while(keyPush != '*' && num <1000 && currentLoc < 4)
    {
        keyPush = KeypadRead();
        if(keyPush != 0 && keyPush != '*' && keyPush != '#')
        {
            num *= 10;
            num += (keyPush-0x30);
            currentLoc++;
            LCDPrintChar(keyPush, LCDline, currentLoc);
            keyPush = 0;
        }
        if(keyPush == '#')
        {
            num = 0;
            currentLoc = 0;
            LCDClearLine(LCDline);
            keyPush = 0;
        }
    }
    return num;
}

/*  
 * Write the liquid tank data to EEPROM
 * Parameters:
 *      tank: the data of type liquidTank structure which will be saved
 *      tankIndex: the index of the liquid tank
 */
void writeEEPROM(struct liquidTank tank, uint8_t tankIndex)
{
    //Each liquidTank struct is 17 bytes long.
    uint8_t addrs = tankIndex<<5;
    uint8_t addrsOffset = 0;
    for(addrsOffset; addrsOffset<=7; addrsOffset++)
        eeprom_write(addrs+addrsOffset, tank.name[addrsOffset]);
    eeprom_write(addrs+addrsOffset, (uint8_t)tank.length>>8);
    addrsOffset++;
    eeprom_write(addrs+addrsOffset, (uint8_t)tank.length&255);
    addrsOffset++;
    eeprom_write(addrs+addrsOffset, (uint8_t)tank.width>>8);
    addrsOffset++;
    eeprom_write(addrs+addrsOffset, (uint8_t)tank.width&255);
    addrsOffset++; 
    eeprom_write(addrs+addrsOffset, (uint8_t)tank.height>>8);
    addrsOffset++;
    eeprom_write(addrs+addrsOffset, (uint8_t)tank.height&255);
}

/*
 *  Read liquid tank data stored in EEPROM
 *  Parameters:
 *      *tank: a pointer to a liquidTank structure
 *      tankIndex: the index of the liquid tank
 */
void readEEPROM(struct liquidTank * tank, uint8_t tankIndex)
{
    uint8_t addrs = tankIndex<<5;
    uint8_t addrsOffset = 0;
    uint16_t temp = 0;
    addrsOffset = 0;
    for(addrsOffset; addrsOffset<=7; addrsOffset++)
        tank->name[addrsOffset] = EEPROM_READ(addrs+addrsOffset);
    
    temp = EEPROM_READ(addrs+addrsOffset);
    addrsOffset++;
    LCDClearDisplay();
    temp =+ EEPROM_READ(addrs+addrsOffset);
    tank->length = temp;
    addrsOffset++;
    temp = EEPROM_READ(addrs+addrsOffset);
    addrsOffset++;
    temp =+ EEPROM_READ(addrs+addrsOffset);
    tank->width = temp;
    addrsOffset++;
    temp = EEPROM_READ(addrs+addrsOffset);
    addrsOffset++;
    temp =+ EEPROM_READ(addrs+addrsOffset);
    tank->height = temp;    
}