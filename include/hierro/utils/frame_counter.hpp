#pragma once

#include <chrono>
#include <optional>
#include <ratio>
#include <thread>

class FrameCounter {
public:
  inline void start() {
    this->start_point = clock.now();
  }

  inline void end() {
    this->end_point = clock.now();
    this->frame_rate =
      1000.0 / delta(this->start_point, this->end_point).count();
  }

  inline double get_frame_rate() {
    return frame_rate;
  }

  inline void set_frame_limit(double frame) {
    this->frame_limit = frame;
  }

  inline void wait_until_limit() {
    if (this->frame_limit && this->frame_limit.value() < frame_rate) {
      this->frame_rate = this->frame_limit.value();
      auto loop_time = delta(this->start_point, this->end_point);
      auto time_per_frame = delta(0, 1.0 / frame_limit.value());
      std::this_thread::sleep_until(this->start_point + time_per_frame);
    }
  }

private:
  inline std::chrono::duration<long, std::ratio<1, 1000>>
  delta(double start, double end) {
    const std::chrono::duration<double> _delta { end - start };
    return std::chrono::duration_cast<std::chrono::milliseconds>(_delta);
  }

  inline std::chrono::duration<long, std::ratio<1, 1000>> delta(
    std::chrono::time_point<std::chrono::steady_clock> start,
    std::chrono::time_point<std::chrono::steady_clock> end
  ) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  }

  std::chrono::steady_clock clock;
  std::chrono::time_point<std::chrono::steady_clock> start_point;
  std::chrono::time_point<std::chrono::steady_clock> end_point;
  double frame_rate;
  std::optional<double> frame_limit;
};

// auto start = clock.now();
// if (this->update())
//   hierro_check(this->render());
// auto end = clock.now();
// const std::chrono::duration<double> _delta { end - start };
// auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(_delta);
//
// auto frame_rate = 1000.0 / delta.count();
//
// if (this->frame_limit && this->frame_limit.value() < frame_rate) {
//   this->frame_rate = this->frame_limit.value();
//   auto time_per_frame =
//     std::chrono::duration_cast<std::chrono::milliseconds>(
//       std::chrono::duration<double>(1000.0 / this->frame_limit.value())
//     );
//   std::this_thread::sleep_for(time_per_frame - delta);
// } else {
//   this->frame_rate = frame_rate;
// }
