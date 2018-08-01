#include "coroutine.h"
#include <gtest/gtest.h>

TEST(CoroutineTest, Traces) {

  EXPECT_TRUE(co::finished());

  std::vector<int> t;

  t.push_back(0);

  auto c1 = co::create([&] {
    t.push_back(2);
    co::yield();
    t.push_back(5);
    co::yield();
    t.push_back(8);
  });

  auto c2 = co::create([&] {
    t.push_back(3);
    co::yield();
    t.push_back(6);
    co::yield();
    t.push_back(9);
  });

  t.push_back(1);

  co::resume(c1);
  co::resume(c2);

  t.push_back(4);

  co::resume(c1);
  co::resume(c2);

  t.push_back(7);

  co::resume(c1);
  co::resume(c2);

  EXPECT_EQ((std::vector<int>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}), t);

  EXPECT_TRUE(co::finished());
}

TEST(CoroutineTest, Dispatch) {

  EXPECT_TRUE(co::finished());

  std::vector<int> t;

  t.push_back(0);

  co::create([&] {
    t.push_back(1);
    co::yield();
    t.push_back(2);
    co::yield();
    t.push_back(3);
  });

  co::create([&] {
    t.push_back(1);
    co::yield();
    t.push_back(2);
    co::yield();
    t.push_back(3);
  });

  co::dispatch();

  EXPECT_EQ((std::vector<int>{0, 1, 1, 2, 2, 3, 3}), t);

  EXPECT_TRUE(co::finished());
}
