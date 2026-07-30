#pragma once

#include <cstdint>

#include "encoder.hpp"
#include "main.h"

class STM32Encoder final : public Encoder
{
 public:
  explicit STM32Encoder(TIM_HandleTypeDef* htim, bool reversed = false)
      : htim_(htim), reversed_(reversed)
  {
    ASSERT(htim_ != nullptr);
    const HAL_StatusTypeDef status = HAL_TIM_Encoder_Start(htim_, TIM_CHANNEL_ALL);
    ASSERT(status == HAL_OK);
    Reset();
  }

  void Reset() override
  {
    __HAL_TIM_SET_COUNTER(htim_, 0);
    previous_counter_ = 0;
    total_ = 0;
  }

  int32_t GetDelta() override
  {
    const int32_t delta = MeasureDelta();
    total_ += delta;
    return delta;
  }

  int32_t GetTotal() override
  {
    const int32_t delta = MeasureDelta();
    total_ += delta;
    return total_;
  }

 private:
  uint32_t ReadCounter() const { return __HAL_TIM_GET_COUNTER(htim_); }

  int32_t MeasureDelta()
  {
    const uint32_t counter = ReadCounter();
    const uint32_t period = __HAL_TIM_GET_AUTORELOAD(htim_);
    const uint64_t modulus = static_cast<uint64_t>(period) + 1ULL;
    int64_t delta =
        static_cast<int64_t>(counter) - static_cast<int64_t>(previous_counter_);
    previous_counter_ = counter;

    if (modulus > 1ULL)
    {
      const int64_t half = static_cast<int64_t>(modulus / 2ULL);
      if (delta > half)
      {
        delta -= static_cast<int64_t>(modulus);
      }
      else if (delta < -half)
      {
        delta += static_cast<int64_t>(modulus);
      }
    }

    if (reversed_)
    {
      delta = -delta;
    }
    return static_cast<int32_t>(delta);
  }

  TIM_HandleTypeDef* htim_;
  bool reversed_;
  uint32_t previous_counter_ = 0;
  int32_t total_ = 0;
};
