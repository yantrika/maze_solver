#include <Arduino.h>
#include "Sensors.hpp"

Sensors::Sensors(){
    // Create the objects dynamically
    sonarLeft = new NewPing(TRIG_L, ECHO_L, MAX_DISTANCE);
    sonarFront = new NewPing(TRIG_F, ECHO_F, MAX_DISTANCE);
    sonarRight = new NewPing(TRIG_R, ECHO_R, MAX_DISTANCE);
}

// Add a destructor to clean up the dynamically allocated objects
Sensors::~Sensors(){
    delete sonarLeft;
    delete sonarFront;
    delete sonarRight;
}

void Sensors::update_vals()
{
    left = sonarLeft->ping_cm();
    right = sonarRight->ping_cm();
    front = sonarFront->ping_cm();
}