#line 1 "/home/bhaskar/study/maze_solver/src/PID.hpp"
#ifndef __PID_HPP
#define __PID_HPP
#include "Motors.hpp"


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