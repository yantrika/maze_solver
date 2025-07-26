#include "utils.hpp"

CircularQueue::CircularQueue(int size) {
        cap = size;
        currSize = 0;
        arr = new int[cap];
        f = 0;
        r = -1;
    }

void CircularQueue::push(int data) {
        if (currSize == cap) {
            return;
        }
        r = (r + 1) % cap;
        arr[r] = data;
        currSize++;
    }

void CircularQueue::pop() {
    if (empty()) {
        return;
    }
    f = (f + 1) % cap;
    currSize--;
}

int CircularQueue::front() {
    if (empty()) {
        return -1;
    }
    return arr[f];
}

bool CircularQueue::empty() {
    return currSize == 0;
}

CircularQueue::~CircularQueue() {
    delete[] arr;
}
