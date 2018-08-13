
#include "mutex.h"

Mutex::Mutex()
{
    pthread_mutex_init(&m, NULL);
}

Mutex::~Mutex()
{
    pthread_mutex_destroy(&m);
}

void Mutex::lock()
{
    pthread_mutex_lock(&m);
}

void Mutex::unlock()
{
    pthread_mutex_unlock(&m);
}

