# TinyUSB

- 上游： https://github.com/hathach/tinyusb
- 文档： https://docs.tinyusb.org/en/stable/
- 定位：跨平台 USB Host/Device 协议栈，包含 CDC、HID、MSC、Audio 等 Class。

## 架构观察

```text
Application
   |
USB Class Drivers (CDC/HID/MSC/Audio)
   |
Device/Host Core
   |
OSAL / Deferred Event Processing
   |
DCD/HCD (MCU USB controller driver)
   |
USB Peripheral / External Host Controller
```

## 值得学习

- Class Driver 与 USB Core 分离。
- MCU 相关代码放在硬件驱动端口层。
- OS 相关同步封装在 OSAL。
- ISR 事件延后到任务/主循环上下文处理。
- examples 按应用类型和 board/BSP 分离。

## 接入检查

1. 确认芯片 USB IP、PHY、Host/Device 模式和 DMA 能力。
2. 确认 DCD/HCD 端口存在且覆盖目标芯片。
3. 配置 endpoint、最大包长、FIFO 和 class 数量。
4. 定义 CDC 数据与应用协议之间的帧边界。
5. 验证 USB reset、suspend/resume、拔插和并发回调。

## 推荐源码阅读

先读 `examples/device/cdc_msc`、`hw/bsp` 目标板配置，再读 `src/device`、Class Driver 和对应 DCD。
