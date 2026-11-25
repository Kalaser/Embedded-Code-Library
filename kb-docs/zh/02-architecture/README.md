# 架构设计模式

这些页面描述可跨平台借鉴的架构思想。它们不是某一个 RTOS 的固定目录标准，应根据芯片能力、团队规模和产品生命周期取舍。

| 模式 | 解决的问题 | 适用范围 |
|---|---|---|
| 分层架构 | 限制依赖方向和硬件耦合 | 几乎所有中大型固件 |
| BSP/HAL/Driver | 复用芯片外设能力并隔离板级差异 | 多板卡/多芯片产品 |
| Component Model | 代码、构建依赖和配置模块化 | SDK、RTOS、中间件、产品应用 |
| Event Loop + State Machine | 管理异步事件、协议流程和错误恢复 | 通信设备、传感器、工厂测试 |
| Device/Session Manager | 处理设备身份、在线状态和会话 | PC 管理多设备、网关、测试系统 |
| Transport Abstraction | 统一不同链路并保留能力差异 | UART/USB/TCP/BLE 多链路产品 |
| Resource Budget | 约束栈、队列、堆、DMA、锁和时延 | 资源受限 MCU 和实时系统 |
| Boot/Update/Recovery | 可靠启动、更新和故障恢复 | 有 OTA/现场维护需求的产品 |
