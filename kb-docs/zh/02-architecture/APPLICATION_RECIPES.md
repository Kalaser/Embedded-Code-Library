# 常见嵌入式应用架构配方

## 1. 传感器采集节点

```text
Timer/DRDY IRQ -> Sensor Driver -> Sample Queue -> Processing -> Storage/Network
```

关键点：采样时间戳、丢样检测、FIFO 深度、DMA/cache、采样和通信优先级分离。

## 2. 设备通信网关

```text
UART/USB/TCP adapters -> Protocol Decoder -> Device Registry -> Command Router -> Services
```

关键点：链路帧边界、设备身份、请求序号、重试、会话失效、并发连接和背压。

## 3. 工厂诊断应用

```text
Host Tool <-> Transport <-> Factory Protocol <-> Test Manager
                                      |             |
                                Device Manager   Test Modules
                                      |             |
                                   Identity      HAL/Driver
```

关键点：测试项注册、能力协商、测试状态、人工交互、超时、日志和结果归档。

## 4. GUI 终端

```text
Input Driver -> UI Event -> View/Controller -> Service API -> Device/Network
                                   |
                              LVGL Renderer
```

关键点：UI 线程归属、跨线程消息、渲染 flush、输入采样、资源生命周期和页面状态。

## 5. OTA 设备

```text
Network/USB -> Download Manager -> Image Validator -> Update State Machine -> Bootloader
```

关键点：可恢复下载、签名验证、分区管理、试运行确认和失败回滚。
