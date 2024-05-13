#pragma once

#include <pthread.h>

class lockClass
{

private:
    int count;
    pthread_mutex_t lock;

public:
    lockClass();
    void increment();
    bool check(int value);
    void clear();
};

lockClass::lockClass()
{
    count = 0;
}

void lockClass::increment()
{
    pthread_mutex_lock(&lock);
    count++;
    pthread_mutex_unlock(&lock);
}

bool lockClass::check(int value)
{
    pthread_mutex_lock(&lock);

    bool check = (count == value);

    pthread_mutex_unlock(&lock);

    return !check;
}

void lockClass::clear()
{
    pthread_mutex_lock(&lock);

    count = 0;

    pthread_mutex_unlock(&lock);
}