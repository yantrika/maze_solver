#ifndef __MOTORS_H
#define __MOTORS_H

class Motors{

    private :
        int IN1 = 8;
        int IN2 = 9;
        int IN3 = 10;
        int IN4 = 11;
        int ENA = 5;
        int ENB = 6; 
        int currspeedR = 100;
        int currspeedL = 100; 
    public :
    Motors();
    void moveForward(int pid);
    void slightLeft();
    void slightRight();
    void turnLeft();
    void turnRight();
    void stopMotors();
    
};

#endif