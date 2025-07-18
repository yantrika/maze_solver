#include <Arduino.h>
// #include "PID.h"
#include "Motors.h"

PID::PID(){}

void PID::change_kp(float changed_val) {

    kp = changed_val;
}

void PID::change
{
    ki = changed_val;
}

float PID::get_error(float left, float right) {
    return left - right;
}

float PID::get_PID_val(float error) {
    float change_err = error - prev_err;
    prev_err = error;
    int_err += error;
    return kp*error + kd*change_err + ki*int_err;
}

void PID::motor_change(float left, float right,Motors* motor_obj)
{
    //It should chnage the motors depending on the thingy
    int pid = PID::get_PID_val(PID::get_error(left,right));

    motor_obj->moveForward(pid);
}
