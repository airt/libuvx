#include <gtest/gtest.h>
#include "coroutine.h"
#include "utilities.h"
#include "loop.h"
#include "fs.h"

TEST(FsTest, File) {

  EXPECT_TRUE(co::finished());

  std::vector<uint64_t> ts;

  auto otx = uv_hrtime();

  for (int i = 0; i < 10; i++) {

    co::create([&, i] {

      auto tx = uv_hrtime();

      auto file = co::fs::open("../license", O_RDONLY);

      co::fs::stat_t stat;
      co::fs::fstat(file, &stat);

      auto buf = co::make_unique_buf(256);
      int64_t offset = 0;

      while (true) {
        int len = co::fs::read(file, &*buf, 1, offset);
        assert(len >= 0);
        if (len == 0) {
          break;
        }
        offset += len;
      }

      EXPECT_EQ(stat.st_size, 1071);
      EXPECT_EQ(stat.st_size, offset);

      co::fs::close(file);

      auto tz = uv_hrtime();
      auto td = tz - tx;
      ts.emplace_back(td);

    });

  }

  co::run();

  auto otz = uv_hrtime();
  auto otd = otz - otx;
  auto tm = *std::max_element(std::begin(ts), std::end(ts));

  EXPECT_GE(otd, tm);
  EXPECT_LE(otd, tm * 2);

  EXPECT_TRUE(co::finished());

}
