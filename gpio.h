#ifndef GPIO_CLASS_H
#define GPIO_CLASS_H
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;
/** RPi-GPIO
	This class simplifies the setting of GPIO pins on a Raspberri Pi in C++
**/
class gpio
{
public:
    gpio();  // create a GPIO object that controls GPIO4 (default)
    gpio(int pin); // create a GPIO object that controls GPIOx, where x is passed to this constructor
    gpio(int pin, bool direction);//Create a GPIO object; use the desired pin direction (input/output)
    ~gpio();
    int enable(); // enables GPIO
    int disable(); // disables GPIO
    int setdirection(bool direction); // Set GPIO Direction
    bool& setval(const bool& val); // Set GPIO Value (output pins) (mirrored with operator=)
    bool getval(); // Get GPIO Value (input/ output pins)
    int getnum(); // return the GPIO number associated with the instance of an object
    bool getdirection(); // get direction set by setdirection() function
    bool& operator=(const bool& val); //set GPIO Value (input/ output pins)

    enum { high = 1, low=0 };
    enum { input = 1, output=0};
private:
    int gpionum; // GPIO number associated with the instance of an object
    string val_str;
    bool direct;
    bool curval;

};
#endif
