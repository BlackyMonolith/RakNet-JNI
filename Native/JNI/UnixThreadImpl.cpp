// The MIT License (MIT)
//
// Copyright (c) 2015 GoMint, BlackyPaw and geNAZt
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

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