#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <thread>
#include "threadsafe_queue.h"

template<class FunctionType>
class ThreadPool {
public:
  ThreadPool();
  ~ThreadPool();

  void submit(FunctionType f);

private:
  void worker_thread();

  // data
  ThreadSafeQueue<FunctionType> queue_;
  int hardware_threads_;
  bool done_;
};

template<class FunctionType>
ThreadPool::ThreadPool():done_(false) {
  hardware_threads_ = std::thread::hardware_concurrency();
}


#endif // THREAD_POOL_H
