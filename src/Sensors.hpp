#ifndef __SENSORS_H
#define __SENSORS_H 

#include <Arduino.h>
#include <NewPing.h>

class Sensors{

    private:

        //To store the sensor pin config
        uint8_t TRIG_L = 12;
        uint8_t ECHO_L = A0; 
        uint8_t TRIG_F = 4;
        uint8_t ECHO_F = 7;
        uint8_t TRIG_R = 2;
        uint8_t ECHO_R = 3;
        uint8_t MAX_DISTANCE = 200;  // in cm

        //Pointers to store the newping stuffings
        NewPing* sonarLeft;
        NewPing* sonarFront;
        NewPing* sonarRight; 

         
    public:

        //To store sensor readings
        float left=0, right=0, front=0;

        //Constructor and destructor
        Sensors();
        ~Sensors();


        //Exposed APIs
        void update_vals();

};

#endif