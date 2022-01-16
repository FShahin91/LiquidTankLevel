# LiquidTankLevel
A system that measures the amount of liquid from multiple liquid tanks.

## Introduction
This is a hobby project made to help in determining the amount of liquid available in a liquid tank in an easy and accurate way.
### Reasons for making this project
In the family's home, there are 4 diesel tanks which are located behind a rather heavy and rusty garage door which requires 2 persons to open. In addition, the method used to determine how much diesel is left in these tanks is one of these two methods:
 1. By looking inside the tanks.
 2. Using what I like to call the stick-o-meter which is simply a wooden broom stick.

So the main purpose of this project is to simply:
* Remove the need to struggle with the door just to check how much diesel there is in the tanks.
* Have a more accurate method to determine the amount of diesel in these tanks.

I must mention that there is still a need to struggle with the door in order to open/close the valves when refilling the tanks. But doing it once is better than twice!

## Technical details
There are 4 tanks which the system will take readings from. Developing a system that reads from one tank only then making 4 of this system will not be financially feasable. The project was made to work with up to 4 tanks with the ability to work with more by making the necessary changes in the code and hardware. The tanks are all cuboid shapes, so the system stores the length, width and height alog with the user name for each tank seperately.

Because the system will take readings from 4 tanks, a 4x16 LCD screen was used. This size was large enough to show all the readings at once and also to include readable instructions for the users when adding or removing an entry to or from the system. The amount of diesel in the tanks is presented in liters and also as a percentage of the total volume of the tanks. The percentage seem to be a more user-friendly method to indicate the amount compared to pure numbers.

Ultrasonics of type HC-SR04 are used in the project due to their cheap price and wide availbiliy. In order to work with 4 sensors and to minimize the number of pins needed to be connected to the MCU, a MUX and deMUX were used. This made it possible to control one ultrasonic at a time and also lower the number of needed pins to control the sensors from 8 pins to 4 pins. A small concern from using this ultrasonic sensor is that it provides readings in centimeters. This will casue some accuracy issues depending on the dimensions of the liquid tank. Another concern is their longivity due to exposure to diesel fumes (or humidity depending on where the system will be used).

The brains of this project is a PIC16F877A MCU from microchip. This 8-bit MCU was chosen due to its cheap price, wide availabilty, nuemrous pins to work with, and it has enough resources for a project of this scale. Coding is done in C using MPLAB X IDE v5.50 and XC8 compiler v2.32, both from microchip. All the coding is done from scratch except for the LCD screen which was taken from [Trion Projects' LCD library](https://trionprojects.org/lcd-library-for-8-bit-pic-microcontrollers/) and modified to suite the project.

## Structure of the project
* The [DatasheetLinks](DatasheetLinks.md) which includes links to all used devices datasheets.
* The [include](include/) directory includes all the used header files in the project. Each header file has a short description of its function.
* The [source](source/) directory includes all the used C code files in the project. Each function in the source files is documented to give as many details as possible on how the function works. There are numerous comments that describe what the code is doing to give the user/reader the best possible understanding of how the code works.
* The [LICENSE](LICENSE) file has full details of the permissions for this project. The project is licensed under GNU GPL v3.0.
* This README file.
