//
// Created by BlackyPaw on 8/2/2015.
//

#ifndef RAKNET_JNI_UNIXTHREADIMPL_H
#define RAKNET_JNI_UNIXTHREADIMPL_H

#ifdef __linux__

#include <pthread.h>

class Thread;

namespace priv {
    class ThreadImpl {

    public:

        ThreadImpl(Thread* thread);
        ~ThreadImpl();

        void start();
        void join();
        void run();
        void terminate();

    private:

        Thread* thread;
        pthread_t handle;


    };
}

#endif

#endif //RAKNET_JNI_UNIXTHREADIMPL_H
