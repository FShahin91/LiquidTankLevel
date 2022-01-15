/* 
 * File:   utility.h
 * Author: FShahin91
 * Comments:
 * This header file and the associated C file includes all functions which will
 * be used multiple times throughout the program.
 * Revision History: v1.0
 */

#ifndef UTILITY_H
#define	UTILITY_H

#include "sm.h"

uint8_t * NumToStr (uint32_t num);
uint16_t numSet(uint8_t LCDline);
uint8_t nameSet(uint8_t * arrName, uint8_t arrSize, uint8_t LCDline);
void readEEPROM(struct liquidTank * tank, uint8_t tankIndex);
void writeEEPROM(struct liquidTank tank, uint8_t tankIndex);

#endif	/* UTILITY_H */
