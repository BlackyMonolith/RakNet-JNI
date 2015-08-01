//
// Created by BlackyPaw on 8/1/2015.
//

#ifdef _WIN32

#include "Win32ThreadImpl.h"
#include "Thread.h"

namespace priv {

    DWORD WINAPI BaseThreadEntry(LPVOID lpParameter);

    ThreadImpl::ThreadImpl(Thread* thread) : thread( thread ) {
        handle = ::CreateThread(0, 0, BaseThreadEntry, this, CREATE_SUSPENDED, 0);
    }

    Thread::~Thread() {
        ::CloseHandle(handle);
    }

    void ThreadImpl::start() {
        ::ResumeThread(handle);
    }

    void ThreadImpl::join() {
        ::WaitForSingleObject(handle, INFINITE);
    }

    void ThreadImpl::run() {
        thread->run();
    }

    void ThreadImpl::terminate() {
        ::TerminateThread(handle, 0);
    }

    DWORD WINAPI BaseThreadEntry(LPVOID lpParameter) {
        ThreadImpl* impl = reinterpret_cast<ThreadImpl*>(lpParameter);
        if (impl == NULL)
            return 0;

        impl->run();
    }

}

#endif