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
/* GPIO Class
 * Purpose: Each object instantiated from this class will control a GPIO pin
 * The GPIO pin number must be passed to the overloaded class constructor
 * Improved by Ian Pudney
 */
class gpio
{
public:
    gpio();  // create a GPIO object that controls GPIO4 (default
    gpio(int pin); // create a GPIO object that controls GPIOx, where x is passed to this constructor
    gpio(int pin, bool direction);
    ~gpio();
    int enable(); // enables GPIO
    int disable(); // disables GPIO
    int setdirection(bool direction); // Set GPIO Direction
    bool& setval(const bool& val); // Set GPIO Value (output pins)
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

gpio::gpio()
{
    this->gpionum = 4; //GPIO4 is default
    val_str = string("/sys/class/gpio/gpio");
    val_str+=static_cast<ostringstream*>( &(ostringstream() << gpionum) )->str();
    val_str+=string("/value");
    enable();
    setdirection(output); //output is default direction
}

gpio::gpio(int gnum)
{
    this->gpionum = gnum;  //Instatiate gpio object for GPIO pin number "gnum"
    val_str = "/sys/class/gpio/gpio";
    val_str+=static_cast<ostringstream*>( &(ostringstream() << gpionum) )->str();
    val_str+="/value";
    enable();
    setdirection(output); //output is default direction
}
gpio::gpio(int gnum,bool direction)
{
    this->gpionum = gnum;  //Instatiate gpio object for GPIO pin number "gnum"
    val_str = "/sys/class/gpio/gpio";
    val_str+=static_cast<ostringstream*>( &(ostringstream() << gpionum) )->str();
    val_str+="/value";
    enable();
    setdirection(direction);
}
gpio::~gpio()
{
    if(direct==output)
    {
        setval(low);
    }
    disable();
}
int gpio::enable()
{
    string exportString;
    exportString+="echo \"";
    exportString+=static_cast<ostringstream*>( &(ostringstream() << gpionum) )->str();
    exportString+="\" > /sys/class/gpio/export";
    system(exportString.c_str());
    return 0;
}

int gpio::disable()
{
    string exportString;
    exportString+="echo \"";
    exportString+=static_cast<ostringstream*>( &(ostringstream() << gpionum) )->str();
    exportString+="\" > /sys/class/gpio/unexport";
    system(exportString.c_str());
    return 0;
}

int gpio::setdirection(bool direction)
{

    string setdir_str ="/sys/class/gpio/gpio";
    setdir_str+=static_cast<ostringstream*>( &(ostringstream() << gpionum) )->str();
    setdir_str+="/direction";
    fstream setdirgpio(setdir_str.c_str()); // open direction file for gpio
        if (setdirgpio < 0){
            cout << " OPERATION FAILED: Unable to set direction of GPIO"<< this->gpionum <<" ."<< endl;
            return -1;
        }
        if(direction)
        {
            setdirgpio<<"in";
        } else
        {
            setdirgpio<<"out";
        }
        setdirgpio.close(); // close direction file
        direct=direction;
        return 0;
}

bool& gpio::setval(const bool& val)
{
    if(direct==input)
    {
        throw "OPERATION FAILED: attempt to set the value of an input pin";
    }
    fstream setvalgpio(val_str.c_str()); // open value file for gpio
        if (setvalgpio < 0){
            throw " OPERATION FAILED: Unable to set the value of GPIO";
        }

        setvalgpio << val ;//write value to value file
        setvalgpio.close();// close value file
        curval=val;
        return curval;
}

bool gpio::getval(){
    int val;
    if(direct==input)
    {
        ifstream getvalgpio(val_str.c_str());// open value file for gpio
        if (getvalgpio < 0){
            cout << " OPERATION FAILED: Unable to get value of GPIO"<< this->gpionum <<" ."<< endl;
            return -1;
        }
        getvalgpio >> val ;  //read gpio value
        getvalgpio.close(); //close the value file
    } else { //if this is an output pin, simply return the currently set output
        val = curval;
    }
    return val;
}
bool gpio::getdirection()
{
    return direct;
}
bool& gpio::operator=(const bool& val)
{
    return setval(val);
}
int gpio::getnum(){

return this->gpionum;

}


#endif

