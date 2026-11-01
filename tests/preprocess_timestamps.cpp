#include "preprocess.h"
#include "log_paths.h"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

void require(bool condition, const char *message)
{
  if (!condition) throw std::runtime_error(message);
}

int main()
{
  auto message = std::make_shared<livox_ros_driver2::msg::CustomMsg>();
  const uint32_t offsets[] = {5000000, 5001389, 4990000, 5004167};
  for (unsigned i = 0; i < 4; ++i) {
    livox_ros_driver2::msg::CustomPoint p;
    p.x = 2.f + i;
    p.y = 1.f;
    p.z = 0.5f;
    p.line = 0;
    p.tag = 0x10;
    p.reflectivity = 10 + i;
    p.offset_time = offsets[i];
    message->points.push_back(p);
  }
  message->point_num = message->points.size();
  Preprocess retained, legacy;
  retained.set(false, AVIA, 0.01, 1);
  legacy.set(false, AVIA, 0.01, 1);
  retained.blind_sqr = legacy.blind_sqr = 0.01 * 0.01;
  retained.preserve_point_timestamps = true;
  PointCloudXYZI::Ptr a(new PointCloudXYZI), b(new PointCloudXYZI);
  retained.process(message, a);
  legacy.process(message, b);
  require(a->size() == 4 && b->size() == 4, "Point selection changed");
  for (unsigned i = 0; i < 4; ++i) {
    require(std::abs(a->points[i].curvature - offsets[i] / 1000000.f) < 1e-6,
            "Supplied per-point timestamp was rewritten");
    require(a->points[i].x == b->points[i].x && a->points[i].intensity == b->points[i].intensity,
            "Timestamp option changed point geometry or intensity");
  }
  require(b->points[0].curvature == 0.f && b->points[1].curvature < 0.01f,
          "Legacy default timestamp behavior changed");
  setenv("LIVO_LOG_DIR", "/tmp/livo-test-owned-output", 1);
  require(livo_log_directory() == "/tmp/livo-test-owned-output/", "Run output escaped its directory");
  std::cout << "Point timestamp regression and run output isolation passed\n";
}
