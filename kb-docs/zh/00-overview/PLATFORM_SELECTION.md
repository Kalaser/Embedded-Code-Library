# 嵌入式平台选型矩阵

本页用于按产品约束选择工程平台，不做“某个 RTOS 全面优于另一个 RTOS”的结论。

| 平台 | 主要目标 | 典型工程形态 | 值得借鉴的架构能力 | 需要重点评估 |
|---|---|---|---|---|
| Cortex-M Bare Metal | 小型 MCU、确定性控制 | Startup + Linker + HAL/LL + main loop | 启动流程、内存布局、ISR/主循环边界 | 中断延迟、轮询负载、功能增长后的复杂度 |
| STM32Cube | STM32 MCU 产品开发 | Cube HAL/LL + BSP + Middleware + Application | 外设初始化、系列级 HAL、板级示例 | Cube 版本、生成代码边界、HAL1/HAL2 差异 |
| FreeRTOS | 需要轻量多任务的 MCU | Kernel + Port + Config + Application | Task/Queue/Semaphore/Timer | tick、优先级、栈水位、内存分配策略 |
| Zephyr | 组件较多、跨板卡、产品化 RTOS | CMake + Kconfig + Devicetree + Drivers | 设备模型、配置裁剪、模块和板卡体系 | 学习成本、Kconfig/Devicetree 复杂度、版本升级 |
| NuttX | POSIX 风格 RTOS、设备文件模型 | Kernel + apps + board/arch + VFS | POSIX 接口、设备驱动、应用仓库分离 | 当前构建配置的 API 支持度、板级移植成熟度 |
| RT-Thread | MCU/IoT、组件与软件包生态 | Kernel + Components + Packages + BSP | FinSH、设备框架、软件包和 BSP | 软件包版本、BSP 维护状态、授权差异 |
| ESP-IDF | Espressif SoC 产品 | Project + Components + sdkconfig | 组件依赖、menuconfig、网络/无线集成 | 芯片系列、IDF 版本、managed component 锁定 |
| Raspberry Pi Pico SDK | RP2040/RP2350 | CMake + SDK + board configuration | 轻量 SDK、PIO、双核和硬件 API | SDK 版本、板卡、Boot Stage 和 linker 配置 |
| Buildroot | 嵌入式 Linux 产品镜像 | defconfig + package + BR2_EXTERNAL | 可重复系统镜像构建、外部产品树 | kernel/rootfs/toolchain 和 package 版本 |
| Yocto Project | 复杂嵌入式 Linux 产品平台 | layers + recipes + machine/distro config | 分层元数据、可扩展产品发行版 | BitBake 学习成本、层优先级、构建时间和缓存 |

## 选型决策

### 小型 MCU、无复杂并发

从裸机或轻量 RTOS 开始。优先确认是否真的需要线程、动态内存和文件系统，不要仅为“架构完整”引入复杂内核。

### 多任务、通信和外设较多

FreeRTOS、RT-Thread、NuttX 或 Zephyr 都可作为候选。应比较现有 BSP、驱动、无线协议栈、开发团队经验与长期维护成本。

### 需要成熟设备模型和多板卡支持

优先研究 Zephyr、NuttX、RT-Thread 的设备与驱动组织方式，不要只比较内核 API。

### Wi-Fi/BLE 深度依赖 Espressif 芯片

ESP-IDF 通常是最直接的起点。项目结构应顺应其 component build system，而不是再复制一套平行构建体系。

### Cortex-A / MPU / Linux

Buildroot 适合较直接、产品配置相对固定的镜像构建；Yocto 适合需要多层复用、多个产品变体、长期发行版管理的团队。具体取舍取决于团队规模和产品生命周期。

## 最低选型输入

选择平台前至少明确：SoC/板卡、RAM/Flash、编译器、连接协议、无线需求、启动/升级方案、安全要求、实时性、文件系统、GUI、量产维护周期和许可证策略。
