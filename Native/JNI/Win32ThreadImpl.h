//
// Created by BlackyPaw on 8/1/2015.
//

#ifndef RAKNET_JNI_WIN32THREADIMPL_H
#define RAKNET_JNI_WIN32THREADIMPL_H

#ifdef _WIN32

#include <windows.h>

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

        ThreadImpl(const ThreadImpl&);
        ThreadImpl& operator=(const ThreadImpl&);

        Thread* thread;
        HANDLE handle;

    };
}

#endif

#endif //RAKNET_JNI_WIN32THREADIMPL_H
