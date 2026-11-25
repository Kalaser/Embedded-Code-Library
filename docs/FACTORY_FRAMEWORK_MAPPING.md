# Factory Framework Mapping

此文档说明如何从 Embedded Code Library 中选取资产来构建 Factory Framework；它不是对现有固件架构的强制规定。

## Suggested responsibility map

| Factory subsystem | Recommended responsibility | Candidate library assets | Platform boundary |
|---|---|---|---|
| Factory Core | 生命周期、启动/停止、全局事件分发 | future `event_loop`, `state_machine` components | OSAL scheduler/queue |
| Device Manager | 设备发现、注册、状态与能力 | future `device_registry` | USB/UART/BLE transport discovery |
| Session Manager | 单设备会话、超时、重连、请求关联 | future `session` component | monotonic clock, transport |
| Transport Manager | 统一收发接口与后端选择 | TinyUSB/lwIP catalog; future transport interface | UART/USB/TCP drivers |
| Protocol | 帧解析、命令编码、版本兼容 | Nanopb catalog; ring buffer | byte stream / packet I/O |
| Test Manager | 测试计划、执行、结果与取消 | future test-runner components | RTOS tasks / events |
| Test Modules | Wi-Fi/BT/IMU/Sensor 设备测试 | product-specific modules | vendor drivers and HAL |
| Diagnostics | 日志、设备信息、故障码 | ecl_status; future logging | syslog/console/flash |

## Recommended development sequence

1. 先定义 `ecl_status_t`、日志接口和组件生命周期约定。
2. 明确 Transport API 的返回语义、超时单位、并发模型和消息所有权。
3. 建立 Device Manager 与 Session Manager 的 ID 绑定和状态机。
4. 将测试执行与具体 Wi-Fi/BT/Sensor 驱动隔离。
5. 使用 Host 单元测试验证协议、状态转换和错误路径，再进行 NuttX 目标板集成验证。

## Avoid

- 不让每个 Test Module 自己管理 socket/UART 生命周期。
- 不使用 IP 地址作为设备永久身份标识。
- 不把阻塞式驱动调用放进不受控的全局事件分发线程。
- 不把 `volatile` 当作线程间同步原语。
- 不将 Host 测试结果视为目标硬件实时性或驱动兼容性证明。
