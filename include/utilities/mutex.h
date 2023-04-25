#pragma once

namespace utilities {

class mutex {
public:
    mutex();
    ~mutex();
    void lock();

    void unlock();

    // remove default copy and assign operators
    mutex(const mutex& other) = delete;
    mutex& operator=(const mutex& other) = delete;
};

template <typename TMutex>
class lock_guard {
public:
    inline lock_guard(TMutex& mutex)
        : mMutex(mutex)
    {
        mMutex.lock();
    }

    inline ~lock_guard()
    {
        mMutex.unlock();
    }

private:
    TMutex& mMutex;
};
}