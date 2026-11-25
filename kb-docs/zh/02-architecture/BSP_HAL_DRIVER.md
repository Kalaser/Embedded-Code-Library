# BSP、HAL 与 Driver 模型

## 三者的职责边界

### BSP（板级支持）

表达具体板卡如何工作：时钟树、Pin Mux、外部晶振、Flash、LED、按键、显示屏、传感器接线、供电使能和板载器件初始化。

### HAL（硬件抽象）

提供芯片外设或硬件能力的统一访问 API，例如 UART、SPI、I2C、GPIO、DMA、ADC。HAL 通常面向芯片系列，不一定天然跨厂商。

### Driver（设备驱动）

实现具体外设或设备功能，包括配置、读写、状态管理、中断处理、DMA 协作和错误恢复。驱动可能建立在 HAL 上，也可能直接使用寄存器或 OS Driver Model。

## 推荐依赖

```text
Application / Service
        |
Generic Device Interface (optional)
        |
Device Driver
        |
HAL / SoC Peripheral Layer
        |
BSP Configuration
        |
Hardware
```

## 设计要点

- Board-specific 参数集中在 BSP，不散落在业务代码。
- HAL API 应表达传输/操作语义，而不是只暴露寄存器地址。
- Driver 对外接口应有清楚的同步/异步语义、超时、错误返回和生命周期。
- DMA 缓冲区必须考虑对齐、Cache 一致性和内存区域。
- 多实例设备不能依赖单一全局状态；使用 context/instance。
- 共享中断与资源应明确并发保护策略。

## 一个常见反模式

在业务代码直接调用 `HAL_UART_Transmit()`，并把 UART 实例、GPIO Pin、DMA Channel 全部硬编码到业务流程。更好的做法是业务依赖 `console_write()`、`transport_send()` 或设备服务 API，底层由 Board/Driver 绑定实际硬件。
