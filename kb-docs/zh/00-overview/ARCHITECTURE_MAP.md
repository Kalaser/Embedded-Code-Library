# 嵌入式架构知识地图

```text
Product / Application
        |
        +-- Product Services
        |     Device Manager / Session Manager / Test Manager
        |     Configuration / Logging / Update / Diagnostics
        |
        +-- Middleware
        |     USB / TCP-IP / BLE / Filesystem / GUI / Serialization
        |
        +-- Platform Adaptation
        |     OSAL / Transport Adapter / Storage Adapter / Display Adapter
        |
        +-- OS / Runtime
        |     RTOS Kernel / POSIX Environment / Embedded Linux
        |
        +-- BSP / HAL / Drivers
        |     Clock / Pinmux / DMA / UART / USB Controller / Sensor
        |
        +-- Hardware
              CPU / Memory / Peripherals / External Components
```

## 核心架构主题

| 主题 | 核心问题 | 对应知识页 |
|---|---|---|
| 分层架构 | 哪一层拥有硬件细节？谁依赖谁？ | `LAYERED_FIRMWARE.md` |
| BSP/HAL/Driver | 板级配置、芯片外设和设备功能如何分离？ | `BSP_HAL_DRIVER.md` |
| 组件化 | 如何管理公共组件、依赖和配置？ | `COMPONENT_MODEL.md` |
| 事件驱动 | 如何从轮询、阻塞 I/O 和回调组织事件流？ | `EVENT_DRIVEN_STATE_MACHINE.md` |
| 设备管理 | 如何处理设备发现、身份、在线状态和生命周期？ | `DEVICE_SESSION_MANAGEMENT.md` |
| 传输抽象 | 如何统一 UART、USB CDC、TCP 等传输方式？ | `TRANSPORT_ABSTRACTION.md` |
| 资源管理 | 如何管理栈、堆、缓冲区、锁和队列？ | `CONCURRENCY_RESOURCE_BUDGET.md` |
| 固件升级 | 如何设计镜像验证、切换、回滚和恢复？ | `BOOT_UPDATE_RECOVERY.md` |

## 设计原则

- 分层不等于层数越多越好；每层都必须承担清晰职责。
- 抽象应隐藏真实变化点，例如 OS、硬件、传输介质和存储后端。
- 设备身份不能只依赖动态 IP 或临时文件描述符。
- ISR 尽量只完成必要的硬件确认和数据搬运，复杂处理延后到线程/任务上下文。
- 对有限资源系统，必须让内存容量、任务栈、队列深度和超时具备可解释的上限。
- 架构图必须对应真实模块和依赖，不要只画概念方框。
