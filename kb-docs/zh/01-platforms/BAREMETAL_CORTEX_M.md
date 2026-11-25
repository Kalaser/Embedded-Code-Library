# Cortex-M 裸机工程

## 定位

裸机工程不运行通用 RTOS，应用通常由启动代码、硬件初始化、中断服务和主循环组成。适合资源有限、任务简单、时序可直接控制的 MCU 应用。

## 典型结构

```text
firmware/
├── CMakeLists.txt
├── cmake/toolchain-arm-none-eabi.cmake
├── startup/                 # 向量表、启动汇编（芯片/工具链相关）
├── linker/                  # Flash/RAM 布局
├── CMSIS/                   # Core/Device headers（若所选 SDK 使用）
├── drivers/                 # 外设驱动
├── bsp/                     # 板卡引脚、时钟、外部器件
├── platform/                # tick、critical section、board API
├── app/                     # 产品业务
└── config/                  # 编译期配置
```

## 启动链路

1. 复位后由芯片启动机制进入向量表。
2. Reset Handler 建立栈、复制 `.data`、清零 `.bss`。
3. 执行 C Runtime 初始化和系统时钟/低层初始化。
4. 调用 `main()`。
5. `main()` 初始化 BSP、驱动和应用。
6. 主循环调度状态机、事件或周期任务；中断只处理必要的实时事件。

实际启动顺序由芯片、启动文件、编译器运行库和厂商 SDK 决定，不应把通用示意当成所有芯片固定实现。

## 常见架构方式

- Super Loop：主循环按固定顺序调用模块，简单但需控制单次执行时间。
- Cooperative Scheduler：任务主动让出执行权，需明确最长不可抢占时间。
- Interrupt + Deferred Work：中断快速处理，后续在主循环执行。
- Tickless/低功耗循环：等待中断前配置唤醒源并管理时钟状态。

## 工程关键文件

- 向量表与 startup 汇编。
- Linker Script：Flash、RAM、栈、堆和特殊段定义。
- Toolchain file：交叉编译器、CPU/FPU/ABI 参数。
- Board clock/pinmux：板级时钟和引脚配置。
- Debug config：SWD/JTAG、下载和调试器配置。

## 适用场景

小型传感器节点、简单控制器、低功耗固件、Bootloader、对 RAM/Flash 极其敏感的应用。

## 主要风险

- 主循环中存在无界阻塞，导致其他任务饥饿。
- ISR 中执行复杂协议解析、日志或动态内存分配。
- 忽略中断与主循环共享数据的同步规则。
- Linker Script 与芯片实际内存不匹配。
- 栈/堆边界和 DMA 缓冲区区域没有通过 map 文件验证。

## 推荐研究

- Arm CMSIS Core/Device。
- STM32Cube 的 Startup、CMSIS、HAL/LL 与 Projects。
- Raspberry Pi Pico SDK 的 CMake 和硬件 API。
- libopencm3 的外设驱动组织方式。
