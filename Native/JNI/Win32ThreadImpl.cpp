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

    ThreadImpl::~ThreadImpl() {
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