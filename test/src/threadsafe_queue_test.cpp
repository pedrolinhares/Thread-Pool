#include "gtest/gtest.h"
#include "threadsafe_queue.h"

class ThreadSafeQueueTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    // initial values
    queue.push(1);
    queue.push(2);
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test
    // (right before the destructor).
  }

  ThreadSafeQueue<int> queue;
};

TEST_F(ThreadSafeQueueTest, clear_queue){
  EXPECT_FALSE(queue.empty());
  queue.clear();
  EXPECT_TRUE(queue.empty());
}
TEST_F(ThreadSafeQueueTest, push_element){
  queue.clear();
  queue.push(1);
  EXPECT_FALSE(queue.empty());
}

TEST_F(ThreadSafeQueueTest, pop_element){
  int value;

  queue.try_pop(value);
  EXPECT_EQ(1, value);

  queue.try_pop(value);
  EXPECT_EQ(2, value);
  EXPECT_TRUE(queue.empty());
  EXPECT_FALSE(queue.try_pop(value));
}

TEST_F(ThreadSafeQueueTest, pop_element_unique_ptr){
  std::unique_ptr<int> value = queue.try_pop();
  EXPECT_EQ(1, *value);

  value = queue.try_pop();
  EXPECT_EQ(2, *value);
  EXPECT_TRUE(queue.empty());
}