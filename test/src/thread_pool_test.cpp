#include "gtest/gtest.h"
#include "thread_pool.h"
#include <functional>
#include <iostream>

class ThreadPoolTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    // initial values
    f1 = [this]() { this->aux1 = 2; };
    f2 = [this]() { this->aux2 = 4; };
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test
    // (right before the destructor).
  }

  int aux1, aux2;
  std::function<void()> f1, f2;
};

TEST_F(ThreadPoolTest, submit_function) {
  {
    ThreadPool<std::function<void()>> pool;
    pool.submit(f1);
    pool.submit(f2);
  } // destructor joins threads

  EXPECT_EQ(2, aux1);
  EXPECT_EQ(4, aux2);
}

TEST_F(ThreadPoolTest, wait_for_threads) {
  ThreadPool<std::function<void()>> pool;
  pool.submit(f1);
  pool.submit(f2);

  pool.wait_for_threads();

  EXPECT_EQ(2, aux1);
  EXPECT_EQ(4, aux2);
}

