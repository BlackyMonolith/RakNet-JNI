//
// Created by BlackyPaw on 8/1/2015.
//

#include "Thread.h"

#include "Win32ThreadImpl.h"
#include "UnixThreadImpl.h"

Thread::Thread() {
    impl = std::shared_ptr<priv::ThreadImpl>( new priv::ThreadImpl( this ) );
}

Thread::~Thread() {

}

void Thread::start() {
    impl->start();
}

void Thread::join() {
    impl->join();
}

void Thread::terminate() {
    impl->terminate();
}