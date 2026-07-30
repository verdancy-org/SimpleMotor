#pragma once

// clang-format off
/* === MODULE MANIFEST V2 ===
module_description: Base interface for simple wheel motor modules
constructor_args: []
template_args: []
required_hardware: []
depends: []
=== END MANIFEST === */
// clang-format on

#include <cstdint>

#include "libxr_def.hpp"

/**
 * @class SimpleMotor
 * @brief 简易轮式电机抽象基类
 * @brief Abstract base class for simple wheel motor modules
 *
 * 该类定义了轮式电机驱动的基本接口，支持闭环速度控制和开环占空比控制。
 * 所有具体的电机驱动（如 DRV8870、L298N 等）应继承此类并实现其纯虚方法。
 * This class defines the basic interface for wheel motor drivers,
 * supporting both closed-loop speed control and open-loop duty cycle control.
 * All concrete motor drivers (e.g., DRV8870, L298N) should inherit from
 * this class and implement its pure virtual methods.
 */
class SimpleMotor
{
 public:
  /**
   * @struct Feedback
   * @brief 电机实时反馈数据结构
   * @brief Motor real-time feedback data structure
   *
   * 包含编码器增量、总计数、当前转速、目标转速、占空比和闭环状态。
   * Contains encoder delta, total count, current speed, target speed,
   * duty cycle and closed-loop status.
   */
  struct Feedback
  {
    int32_t delta = 0;          ///< 编码器增量脉冲数 / Encoder delta pulse count
    int32_t total = 0;          ///< 编码器累计脉冲数 / Encoder total pulse count
    float speed = 0.0F;         ///< 当前转速 / Current speed
    float target_speed = 0.0F;  ///< 目标转速 / Target speed
    float duty = 0.0F;          ///< 当前占空比 / Current duty cycle
    bool closed_loop = true;    ///< 闭环使能标志 / Closed-loop enabled flag
  };

  /**
   * @brief 使能电机输出
   * @brief Enables the motor output
   * @return 操作结果，成功返回 ErrorCode::OK / ErrorCode::OK on success
   *
   * 激活电机驱动输出，使电机能够响应控制信号。
   * Activates the motor driver output so the motor can respond to
   * control signals.
   */
  virtual LibXR::ErrorCode Enable() = 0;

  /**
   * @brief 禁用电机输出
   * @brief Disables the motor output
   *
   * 关闭电机驱动输出，电机将进入惰性滑行状态。
   * Deactivates the motor driver output; the motor will coast freely.
   */
  virtual void Disable() = 0;

  /**
   * @brief 放松电机（高阻态）
   * @brief Relaxes the motor (high-impedance state)
   *
   * 将电机驱动输出置为高阻态，与 Disable 类似但实现方式不同。
   * Sets the motor driver output to a high-impedance state,
   * similar to Disable but with a different implementation.
   */
  virtual void Relax() = 0;

  /**
   * @brief 设置目标转速（闭环模式）
   * @brief Sets the target speed (closed-loop mode)
   * @param target_speed 目标转速 / Target speed
   *
   * 在闭环速度控制模式下设定目标转速。
   * Sets the target speed in closed-loop speed control mode.
   */
  virtual void SetTargetSpeed(float target_speed) = 0;

  /**
   * @brief 设置开环占空比
   * @brief Sets the open-loop duty cycle
   * @param duty 占空比（0.0 ~ 1.0） / Duty cycle (0.0 to 1.0)
   *
   * 在开环控制模式下直接设置 PWM 占空比。
   * Directly sets the PWM duty cycle in open-loop control mode.
   */
  virtual void SetOpenLoopDuty(float duty) = 0;

  /**
   * @brief 更新电机控制状态
   * @brief Updates the motor control state
   * @param dt_seconds 距上次更新的时间间隔（秒） / Time delta since last update (seconds)
   * @return 操作结果，成功返回 ErrorCode::OK / ErrorCode::OK on success
   *
   * 周期调用以执行速度闭环计算或开环调节。
   * Periodically called to perform closed-loop speed calculation or
   * open-loop regulation.
   */
  virtual LibXR::ErrorCode Update(float dt_seconds) = 0;

  /**
   * @brief 获取电机实时反馈
   * @brief Gets the motor real-time feedback
   * @return 指向反馈数据的常引用 / Const reference to the feedback data
   */
  virtual const Feedback& GetFeedback() const = 0;

  /**
   * @brief 获取当前目标转速
   * @brief Gets the current target speed
   * @return 目标转速 / Target speed
   */
  virtual float GetTargetSpeed() const = 0;

  /**
   * @brief 检查是否有活跃的控制指令
   * @brief Checks whether an active control command is present
   * @return 有活跃指令返回 true，否则返回 false / true if active command exists
   *
   * 用于判断电机是否正在接收有效的控制输入。
   * Used to determine whether the motor is receiving valid control input.
   */
  virtual bool HasActiveCommand() const = 0;
};
