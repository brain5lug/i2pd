#include <exception>

#include "Service.h"

namespace i2p {

Service::~Service()
{
    // thread must be stoped and joined by this point normally
    // the probles if thread is running it likely to use members
    // of the nested class but the time this function is called
    // nested desctructor is already completed.
}

// start is separated from constructor as it must be possible to
// complete initialization of the nested classes
void Service::Start()
{
    thrd_ = std::thread([this](){ this->Run(); });
}

// just a way to noficate a thread to stop
void Service::NotifyStop() noexcept
{
    must_stop_.store(true, std::memory_order_relaxed);
    // the point is now allow notify_all happen before
    // notify_all, might be overkill must work
    std::atomic_thread_fence(std::memory_order_seq_cst);
    WakeUp();
}

void Service::Stop()
{
    if (thrd_.joinable())
    {
        NotifyStop();
        thrd_.join();
    }
    PostStop();
}

void Service::PreStart() {}

void Service::PostStop() {}

} // namespace i2p
