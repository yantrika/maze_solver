#line 1 "/home/bhaskar/study/maze_solver/src/PID.cpp"
#include <Arduino.h>
#include "PID.hpp"
#include "Motors.hpp"

PID::PID() {}

void PID::change_kp(float changed_val)
{
    kp = changed_val;
}

void PID::change_kd(float changed_val)
{
    kd = changed_val;
}
void PID::change_ki(float changed_val)
{
    if (changed_val < 0)
    {
        changed_val = 0; // Ensure ki is non-negative
    }
    if (changed_val > 1)
    {
        changed_val = 1; // Ensure ki does not exceed 1
    }
}

float PID::get_error(float left, float right)
{
    return left - right;
}

float PID::get_PID_val(float error)
{
    float change_err = error - prev_err;
    prev_err = error;
    int_err += error;
    return kp * error + kd * change_err + ki * int_err;
}

void PID::motor_change(float left, float right, Motors *motor_obj)
{
    // It should chnage the motors depending on the thingy
    int pid = PID::get_PID_val(PID::get_error(left, right));

    motor_obj->moveForward(pid);
}
