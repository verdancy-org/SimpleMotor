#pragma once

#include <cstdint>

#include "libxr_def.hpp"

/**
 * @class Encoder
 * @brief 编码器抽象基类
 * @brief Abstract base class for position/speed encoders
 *
 * 该类定义了增量式编码器的基本接口，所有具体编码器驱动应继承此类
 * 并实现 Reset、GetDelta 和 GetTotal 方法。
 * This class defines the basic interface for incremental encoders. All concrete
 * encoder drivers should inherit from this class and implement the Reset,
 * GetDelta and GetTotal methods.
 */
class Encoder
{
 public:
  /**
   * @brief 复位编码器计数值
   * @brief Resets the encoder counter to zero
   *
   * 将编码器的总计数清零，增量计数不受影响。
   * Resets the total counter to zero. The delta counter is unaffected.
   */
  virtual void Reset() = 0;

  /**
   * @brief 获取上次读取以来的增量值
   * @brief Gets the delta count since the last read
   * @return 增量脉冲数 / Delta pulse count
   *
   * 返回自上次调用该方法以来编码器转过的脉冲数，
   * 正值表示正向旋转，负值表示反向旋转。
   * Returns the number of pulses the encoder has moved since this method
   * was last called. Positive values indicate forward rotation, negative
   * values indicate reverse rotation.
   */
  virtual int32_t GetDelta() = 0;

  /**
   * @brief 获取累计总脉冲数
   * @brief Gets the total accumulated pulse count
   * @return 累计脉冲数 / Total accumulated pulse count
   *
   * 返回从上电或上次复位以来编码器转过的累计脉冲数。
   * Returns the total accumulated pulse count since power-up or the last
   * reset.
   */
  virtual int32_t GetTotal() = 0;
};
