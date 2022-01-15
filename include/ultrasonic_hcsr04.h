/* 
 * File:   ultrasonic_hcsr04.h
 * Author: Faris Shahin
 *
 * Library for HC-SR04 ultrasonic
 * Revision History: v1.0
 */

#ifndef ULTRASONIC_HCSR04_H
#define	ULTRASONIC_HCSR04_H
//Set the port and trigger/echo pins
volatile uint8_t *US_TRIS = &TRISA;
volatile uint8_t *US_DATA = &PORTA;
uint8_t TRIG_PIN = 4;
uint8_t ECHO_PIN = 1;

void UltraSonicInit();
uint16_t UltraSonicPing(uint8_t * TMRCount);

#endif	/* ULTRASONIC_HCSR04_H */
