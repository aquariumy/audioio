
#include "pthread.h"

class Mutex
{

    public:
        Mutex();
        ~Mutex();
        void lock();
        void unlock();

    private:
        mutable pthread_mutex_t m;

};

