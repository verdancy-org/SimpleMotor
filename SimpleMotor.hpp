#pragma once

#include <cstdint>

#include "libxr_def.hpp"

class SimpleMotor
{
 public:
  struct Feedback
  {
    int32_t delta = 0;
    int32_t total = 0;
    float target_delta = 0.0F;
    float duty = 0.0F;
    bool closed_loop = true;
  };

  virtual LibXR::ErrorCode Enable() = 0;
  virtual void Disable() = 0;
  virtual void Relax() = 0;
  virtual void SetTargetDelta(float target_delta) = 0;
  virtual void SetOpenLoopDuty(float duty) = 0;
  virtual LibXR::ErrorCode Update(float dt_seconds) = 0;
  virtual const Feedback& GetFeedback() const = 0;
  virtual float GetTargetDelta() const = 0;
  virtual bool HasActiveCommand() const = 0;
};
