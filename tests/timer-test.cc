#include <gtest/gtest.h>
#include "coroutine.h"
#include "utilities.h"
#include "timer.h"
#include "loop.h"

TEST(TimerTest, Sleep) {

  EXPECT_TRUE(co::finished());

  constexpr uint64_t timeout = 100;

  auto otx = uv_hrtime();

  for (int i = 0; i < 10; i++) {

    co::create([=] {

      auto tx = uv_hrtime();

      co::sleep(timeout);

      auto tz = uv_hrtime();
      auto td = tz - tx;

      EXPECT_GE(td / 1e6, timeout / 2);
      EXPECT_LE(td / 1e6, timeout * 2);

    });

  }

  co::run();

  auto otz = uv_hrtime();
  auto otd = otz - otx;

  EXPECT_GE(otd / 1e6, timeout / 2);
  EXPECT_LE(otd / 1e6, timeout * 2);

  EXPECT_TRUE(co::finished());

}
