//
// Created by BlackyPaw on 8/1/2015.
//

#ifndef RAKNET_JNI_THREAD_H
#define RAKNET_JNI_THREAD_H

#include <memory>

namespace priv {
    class ThreadImpl;
}

class Thread {

public:

    /// @brief Constructs a new thread
    Thread();
    virtual ~Thread();

    /// @brief Starts the thread by running its run method.
    void start();

    /// @brief Joins the thread until it exits.
    void join();

    /// @brief To be implemented by derived classes; Runs the thread's logic.
    virtual void run() = 0;

    /// @brief Terminates the thread's execution.
    void terminate();

private:

    // Prevent copies:
    Thread(const Thread&);
    Thread& operator=(const Thread&);

    std::shared_ptr<priv::ThreadImpl> impl;

};


#endif //RAKNET_JNI_THREAD_H
