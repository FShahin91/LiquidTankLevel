#include "config.h"

/*
 * Initializes the data of the liquid tanks and the size of the user name array.
 * Returns:
 *      The next state to be executed (hardcoded as view())
 * Notes:
 * The function checks the EEPROM if there are already data present there from
 * previous uses of the system. If such data exists, it is read from EEPROM.
 * Otherwise, the data of the liquid tanks is initialized to be zero/empty.
 */
uint8_t init()
{
    arrSize = (sizeof(liquidTanks[0].name)-1)/sizeof(uint8_t); //Exclude \0 in calculation

    //Check if liquid tanks are initialized
    if(eeprom_read(0xaa)!= 0x2a)
    {
        for(uint8_t i=0; i<4; i++)
        {
            for(uint8_t j=0; j<=arrSize; j++)
                liquidTanks[i].name[j] = ' ';
            liquidTanks[i].name[6] = '\0';
            liquidTanks[i].height = 0;
            liquidTanks[i].width = 0;
            liquidTanks[i].length = 0;
            writeEEPROM(liquidTanks[i],i);

        }
        eeprom_write(0xaa, 0x2a);
    }
    else
        for(uint8_t i=0; i<4; i++)
            readEEPROM(&liquidTanks[i],i);

        return view();
}

/*
 * The idle state. The system takes readings from the ultrasonic(s) after
 * around 5 seconds.
 * Returns:
 *      The next state; idle by default or view() after around 5 seconds
 */
uint8_t idle (void)
{
    if(TMR0of >= 250)  //Take readings after around 5 seconds
    {
        TMR0of = 0;
        return view();
    }
    return ST_IDLE;
}

/*
 * The view state where the data of the liquid tanks is displayed on the LCD
 * screen
 * Returns:
 *       The next state to be executed (hardcoded as idle())
 */
uint8_t view(void)
{
    uint16_t distVal;    //Used to store the distance
    uint8_t lineNum = 1; //Used to indicate the current line on the LCD
    uint8_t count = 0;
    uint32_t numLiters; //Used to store the liters currently contained in a tank in numerical format
    uint32_t totalLiters;   //Used to store the total liters a liquid tank can contain
    uint8_t percLiters; //Used to store the percentage of liters currently contained in a tank
    LCDClearDisplay();
    for(count; count <4; count++)   //Loop through the liquid tanks
    {
        if (liquidTanks[count].name[0] != ' ')
        {
            switch (count)
            //Set the ultrasonic sensor from which to read
            {
                case 0:
                    PORTA &= 0xf9;
                    break;
                case 1:
                    PORTA &= 0xfb;
                    PORTA |= 0x02;
                    break;
                case 2:
                    PORTA &= 0xfd;
                    PORTA |= 0x04;
                    break;
                case 3:
                    PORTA |= 0x06;
                    break;   
            }
            distVal = UltraSonicPing(&TMR0of);  //Read from the ultrasonic
            if(distVal != 0 && distVal <= liquidTanks[count].height)
            {
                //For some reason, having the equation in one lines doesn't seem to work.
                //Causes might be related to the size of variables.
                //Casting everything to uint32_t didn't solve it.
                numLiters = liquidTanks[count].length/10;
                numLiters = (numLiters*liquidTanks[count].width)/100;
                totalLiters = numLiters * liquidTanks[count].height;
                numLiters = numLiters*(liquidTanks[count].height-distVal);
                percLiters = (numLiters*100)/totalLiters;                
            }
            else
            {
                numLiters = 0;
                percLiters = 0;
            }
            //Print the data to the LCD screen
            LCDPrintString(liquidTanks[count].name, lineNum, 1);
            LCDPrintString(NumToStr(numLiters),lineNum,8);
            LCDPrintString("L|", lineNum,12);
            LCDPrintString(NumToStr(percLiters), lineNum,14);
            LCDPrintChar('%', lineNum, 16);
            lineNum++;
        }
    }
    
    //If there's no data stored in liquidTanks structure, display a message
    if(count == 4 && lineNum == 1)
    {
    //From the way the LCD array is organized, writing in the next order
    //should be slightly faster (1st line, then 3rd, then 2nd, then 4th)
        LCDPrintString("No Data.", 1 ,1);
        LCDPrintString("options.",3,1);
        LCDPrintString("Press * for",2,1);        
    }
    //Enable timer0 interrupt to continuously read from the ultrasonics around 5s in idle state
    TMR0IE = 1;
    
    return ST_IDLE;
}

/*
 * The options state where the user is presented with a number of options to
 * choose from.
 * Returns:
 *      The next state to be executed (hardcoded as options())
 */
uint8_t options(void)
{
    //From the way the LCD array is organized, writing in the next order
    //should be slightly faster (1st line, then 3rd, then 2nd, then 4th)
    LCDClearDisplay();
    LCDPrintString("1.Add/Edit entry",1,1);
    LCDPrintString("3.Exit",3,1);
    LCDPrintString("2.Delete entry",2,1);

    return ST_OPTIONS;
}

/*
 * The add/edit state where the user can add a new entry or edit an existing one
 * Returns:
 *      The next state to be executed (hardcoded as addEditState())
 */
uint8_t addEditEntry (void)
{
    uint8_t sensor = 10;
    uint8_t returnVal = 0;
    
    while(sensor > 4)
    {
        LCDClearDisplay();
        LCDPrintString("Enter sensor num", 1 ,1);
        LCDPrintString("from 1 to 4:",2,1);
        LCDPrintString("*: Confirm",4,1);
        LCDSetPos(1,3);
        LCDCursorOn();
        LCDCursorBlinkOn();
        sensor = numSet(3);
        if(sensor>4)    //In case the application is modified to use less or more sensors, do the necessary changes here
        {
            LCDClearDisplay();
            LCDCursorBlinkOff();
            LCDCursorOff();
            LCDPrintString("Number should be",1,1);
            LCDPrintString("from 1 to 4!",2,1);
            __delay_ms(2500);
        }
    }

    while(returnVal != 1)
    {
        LCDClearDisplay();
        LCDPrintString("Enter name: ",1,1);
        LCDPrintString("2/8: Up/Down",3,1);
        LCDPrintString(liquidTanks[sensor-1].name,2,1);
        LCDPrintString("4/6: Left/Right",4,1);    
        LCDSetPos(1,2);
        returnVal = nameSet(liquidTanks[sensor-1].name, arrSize,2);
        if(returnVal == 2)
        {
            LCDClearDisplay();
            LCDPrintString("ERROR: Name must",1,1);
            LCDPrintString("space.",3,1);
            LCDPrintString("not start with a",2,1);
            __delay_ms(2500);
        }
    }    
    
    LCDClearDisplay();
    LCDPrintString("Enter length cm:",1,1);
    LCDPrintString("#: Reset",3,1);
    LCDPrintString("*: Confirm",4,1);
    LCDSetPos(1,2);
    liquidTanks[sensor-1].length = numSet(2);
    
    LCDClearDisplay();
    LCDPrintString("Enter width cm:",1,1);
    LCDPrintString("#: Reset",3,1);
    LCDPrintString("*: Confirm",4,1);
    LCDSetPos(1,2);
    liquidTanks[sensor-1].width = numSet(2);
    
    LCDClearDisplay();
    LCDPrintString("Enter height cm:",1,1);
    LCDPrintString("#: Reset",3,1);
    LCDPrintString("*: Confirm",4,1);
    LCDSetPos(1,2);
    liquidTanks[sensor-1].height = numSet(2);
    
    LCDCursorBlinkOff();
    LCDCursorOff();
    LCDClearDisplay();
    LCDPrintString("Successful!",1,1);
    LCDPrintString("continue.",3,1);
    LCDPrintString("Press '#' to",2,1);
    
    //Update EEPROM
    writeEEPROM(liquidTanks[sensor-1], sensor-1);
    
    return ST_ADD_EDIT;
}

/*
 * The state where the user can delete an entry.
 * Returns:
 *      The next state to be executed (hardcoded as deleteEntry()) 
 */
uint8_t deleteEntry(void)
{
    uint8_t index;
    uint8_t empty = 0;
    uint8_t keypress = '8';
    //Check to see if the liquid tanks data is empty
    for(index = 0; index <4; index ++)
    {
        if(liquidTanks[index].length == 0)
            empty++;
    }
    if(empty == 4 && index == 4)
    {
        LCDClearDisplay();
        LCDPrintString("No entries to", 1, 1);
        LCDPrintString("Press '#' key to",3,1);
        LCDPrintString("delete.", 2, 1);
        LCDPrintString("continue.", 4, 1);
        return ST_DEL;
    }
    index = 0;
    LCDClearDisplay();
    LCDPrintString("Choose entry:",1,1);
    LCDPrintString("2/8: Up/Down",3,1);
    LCDPrintString("*: Select",4,1);
    while(keypress != '*')
    {
        //Go through entries until you find a valid one
        while(liquidTanks[index].name[0] == ' ')
        {
            if(keypress == '8')
            {
                index++;
                if (index >= 4)
                    index = 0;
            }
            if(keypress == '2')
            {
                index--;        //uint8 underflows to 255
                if(index > 200)
                    index = 3;
            }
        }
 
        LCDClearLine(2);
        //KeypadRead always returns a value.
        //A delay is added to lower the visual effect of clearing the lines continuously
        LCDPrintString(liquidTanks[index].name, 2,1);
        __delay_ms(500);
        
        keypress = KeypadRead();
        if(keypress == '8')
            {
                index++;
                if (index >= 4)
                    index = 0;
            }
        if(keypress == '2')
        {
            index--;        //uint8 underflows to 255
            if(index > 200)
                index = 3;
        }
    }
    //Entry is selected. Reset entry to empty/0
    liquidTanks[index].height = 0;
    liquidTanks[index].length = 0;
    liquidTanks[index].width = 0;
    for(uint8_t i=0; i<=arrSize; i++)
            liquidTanks[index].name[i] = ' ';
    LCDClearDisplay();
    LCDPrintString("Entry deleted!",1,1);
    LCDPrintString("continue.",3,1);
    LCDPrintString("Press '#' key to",2,1);
    
    //update EEPROM
    writeEEPROM(liquidTanks[index], index);
    return ST_DEL;
}

/*
 *  A function to get events derived from pressing the keypad.
 *  Returns:
 *      The event mapped to the pressed key
 */
uint8_t getEvent(void)
{
    uint8_t keypress = 0;
    keypress = KeypadRead();
    switch(keypress)
    {
        case '1':
            return EV_KEY_ONE;
        case '2':
            return EV_KEY_TWO;
        case '3':
            return EV_KEY_THREE;
        case '4':
            return EV_KEY_FOUR;
        case '*':
            return EV_KEY_STAR;
        case '#':
            return EV_KEY_HASH;
        case 0:
            return EV_KEY_NONE;
        default:
            return EV_ANY;
    }     
}