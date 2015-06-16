/******************************************************************************
 * A simple thread pool implementation.
 * Copyright (C) 2015  LDSC/UENF
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *****************************************************************************/

#ifndef THREADSAFE_QUEUE_H
#define THREADSAFE_QUEUE_H

#include <deque>
#include <mutex>
#include <memory>

template<class T>
class ThreadSafeQueue {
public:
  void push(T new_value);
  bool try_pop(T& value);
  std::unique_ptr<T> try_pop();
  bool empty() const;
  void clear();

private:
  std::deque<T> queue_;
  mutable std::mutex mutex_;
};

////////////////////////////////////
/// Template class implementation //
////////////////////////////////////

template<class T>
void ThreadSafeQueue<T>::push(T new_value) {
  std::lock_guard<std::mutex> lk(mutex_);
  queue_.push_back(std::move(new_value));
}

template<class T>
bool ThreadSafeQueue<T>::try_pop(T& value) {
  std::lock_guard<std::mutex> lk(mutex_);

  if (queue_.empty())
    return false;

  value = std::move(queue_.front());
  queue_.pop_front();

  return true;
}

template<class T>
std::unique_ptr<T> ThreadSafeQueue<T>::try_pop() {
  std::lock_guard<std::mutex> lk(mutex_);

  if (queue_.empty())
    return std::make_unique<T>();

  auto value = std::make_unique<T>(std::move(queue_.front()));
  queue_.pop_front();

  return value;
}

template<class T>
bool ThreadSafeQueue<T>::empty() const {
  std::lock_guard<std::mutex> lk(mutex_);
  return queue_.empty();
}

template<class T>
void ThreadSafeQueue<T>::clear() {
  std::lock_guard<std::mutex> lk(mutex_);
  queue_.clear();
}

#endif // THREADSAFE_QUEUE_H
