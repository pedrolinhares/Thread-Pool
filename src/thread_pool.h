#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <thread>
#include <vector>
#include "threadsafe_queue.h"

template<class FunctionType>
class ThreadPool {
public:
  ThreadPool();
  ~ThreadPool();

  void submit(FunctionType f);
  void wait_for_threads();
private:
  void worker_thread();

  // data
  bool done_;
  int num_threads_;
  ThreadSafeQueue<FunctionType> queue_;
  std::vector<std::thread> threads_;
};

  ///////////////////////////////
  //thread pool implementation //
  ///////////////////////////////

template<class FunctionType>
ThreadPool<FunctionType>::~ThreadPool() {
  done_ = true;

  for (auto& thread : threads_) {
    if (thread.joinable())
      thread.join();
  }
}

template<class FunctionType>
ThreadPool<FunctionType>::ThreadPool():done_(false) {
  // get number of cores
  const auto hardware_threads = std::thread::hardware_concurrency();
  num_threads_ = (hardware_threads != 0)? hardware_threads : 4;

  // because the creation of threads may throw an exception
  try {
    for (unsigned i = 0; i < num_threads_; ++i)
      threads_.push_back(std::thread(&ThreadPool::worker_thread, this));
  } catch (...) {
    done_ = true;
    throw;
  }
}

template<class FunctionType>
void ThreadPool<FunctionType>::worker_thread() {
  while (!done_ || !queue_.empty()) {
    FunctionType task;

    if (queue_.try_pop(task)) {
      task();
    } else {
      std::this_thread::yield();
    }
  }
}

template<class FunctionType>
void ThreadPool<FunctionType>::submit(FunctionType f) {
  queue_.push(f);
}

template<class FunctionType>
void ThreadPool<FunctionType>::wait_for_threads() {
  done_ = true;

  for (auto& thread : threads_) {
    if (thread.joinable())
      thread.join();
  }
}

#endif // THREAD_POOL_H
