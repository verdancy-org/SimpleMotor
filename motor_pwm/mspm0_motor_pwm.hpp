#pragma once

#include <cstdint>

#include "mspm0_pwm.hpp"

namespace SimpleMotorModule
{

class MSPM0MotorPWM final : public LibXR::PWM
{
 public:
  MSPM0MotorPWM(LibXR::MSPM0PWM::Resources resources, uint32_t force_low_mask,
                uint32_t set_by_octl_mask)
      : pwm_(resources),
        timer_(resources.timer),
        force_low_mask_(force_low_mask),
        set_by_octl_mask_(set_by_octl_mask)
  {
    (void)Disable();
  }

  LibXR::ErrorCode SetDutyCycle(float value) override { return pwm_.SetDutyCycle(value); }

  LibXR::ErrorCode SetConfig(LibXR::PWM::Configuration config) override
  {
    return pwm_.SetConfig(config);
  }

  LibXR::ErrorCode Enable() override
  {
    DL_TimerA_setCCPOutputDisabledAdv(timer_, set_by_octl_mask_);
    return pwm_.Enable();
  }

  LibXR::ErrorCode Disable() override
  {
    (void)pwm_.SetDutyCycle(0.0F);
    DL_TimerA_setCCPOutputDisabledAdv(timer_, force_low_mask_);
    return LibXR::ErrorCode::OK;
  }

 private:
  LibXR::MSPM0PWM pwm_;
  GPTIMER_Regs* timer_;
  uint32_t force_low_mask_;
  uint32_t set_by_octl_mask_;
};

}  // namespace SimpleMotorModule
