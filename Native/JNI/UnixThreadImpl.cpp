//
// Created by BlackyPaw on 8/2/2015.
//

#ifdef __linux__

#include "UnixThreadImpl.h"
#include "Thread.h"

namespace priv {

    void* UnixThreadEntryPoint(void* arg);

    ThreadImpl::ThreadImpl( Thread* thread ) : thread( thread ) {

    }

    ThreadImpl::~ThreadImpl() {

    }

    void ThreadImpl::start() {
        if ( handle != 0 )
            return;

        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

        pthread_create(&handle, &attr, UnixThreadEntryPoint, (void*)this);
        if ( handle == 0 )
            return;

        pthread_attr_destroy(&attr);
    }

    void ThreadImpl::join() {
        if ( handle != 0 ) {
            void* status;
            pthread_join(handle, &status);
        }
    }

    void ThreadImpl::run() {
        thread->run();
    }

    void ThreadImpl::terminate() {
        if ( handle != 0 ) {
            pthread_cancel(handle);
        }
    }

    void* UnixThreadEntryPoint(void* arg) {
        pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

        ThreadImpl* impl = reinterpret_cast<ThreadImpl*>(arg);
        if ( impl == nullptr )
            return (void*)0;

        impl->run();

        pthread_exit((void*)0);
        return (void*)0;
    }
}

#endif