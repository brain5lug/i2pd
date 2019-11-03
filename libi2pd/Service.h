#ifndef SERVICE_H
#define SERVICE_H

#include <thread>
#include <atomic>
#include <memory>
#include <condition_variable>

namespace i2p {

class Service
{
public:    
    Service() {}
    virtual ~Service();

    //non-copyable or movable
    Service(const Service& orig) = delete;
    Service& operator=(const Service& orig) = delete;
    Service(Service&& orig) = delete;
    Service& operator=(Service&& orig) = delete;

    void Start();
    void NotifyStop() noexcept;
    void Stop();

protected:
    bool is_active() noexcept { return !must_stop_.load(std::memory_order_acquire); }

private:

    std::atomic_bool          must_stop_;
    std::thread               thrd_;

    // place your code here
    // can be overriden, but cannot be called directly
    virtual void Run() noexcept = 0;

    // wake the thread somehow to check if must stop
    virtual void WakeUp() noexcept = 0;

    // the action performed before the thread is started
    // at the managment thread context
    // has empty body by default
    virtual void PreStart();

    // the action performed after the thread is stopped
    // at the managnebt thread context
    // has empty body by default
    virtual void PostStop();
};

} // namespace i2p

#endif /* SERVICE_H */
