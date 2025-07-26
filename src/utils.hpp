#ifndef __UTILS_HPP
#define __UTILS_HPP
#include <stdlib.h>

class CircularQueue{

    //define the class-only params
    int* arr;
    int currSize, cap;
    int f = 0, r = -1;

    public:
    
    //Constructor
    CircularQueue(int size);
    ~CircularQueue();

    //Functions
    void push(int data);
    void pop();
    int front();

    private:

    bool empty();

};

#endif