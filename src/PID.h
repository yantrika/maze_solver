#ifndef __PID_H
#define __PID_H
#include "Motors.h"


class PID{

    private:
    short kp, kd, ki;
    float prev_err=0;
    float int_err =0;

    public:
    PID();
    void change_kp(float changed_val);
    void change_kd(float changed_val);
    void change_ki(float changed_val);

    float get_error(float left, float right);
    float get_PID_val(float error);
    void motor_change(float left, float right,Motors* motor_obj);
};

#endif