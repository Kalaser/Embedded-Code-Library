# Transport 抽象架构

## 目标

将“如何传输字节”与“字节代表什么协议/命令”分开。UART、USB CDC、TCP、BLE GATT 是不同传输介质，不应把 WebSocket 或 TCP 设计成业务框架的核心。

## 分层

```text
Business / Command Service
          |
Protocol Framing & Session
          |
Transport Interface
   +------+-------+-------+
   |              |       |
 UART Adapter  USB CDC  TCP Socket
   |              |       |
 Driver         USB Stack  Network Stack
```

## 接口建议

通用接口应有：open/start、send、set_rx_callback 或 read、close/stop、get_capabilities。实际设计中还需要约定：

- 数据是否复制，还是借用调用者缓冲区。
- 异步发送的完成通知和缓冲区释放时机。
- 接收回调运行在 ISR、驱动线程还是事件循环上下文。
- 最大帧长、MTU、流控和背压行为。
- 超时、取消、断线和错误码语义。
- 线程安全和并发 send 的顺序保证。

## 不应过度统一的差异

TCP 是字节流，不能假设一次 recv 对应一帧；UART 可能涉及 DMA 环形缓冲；USB CDC 有 USB transfer 和 endpoint 生命周期；BLE GATT 有 MTU 与连接参数约束。抽象接口要保留这些能力差异，而不是假装所有链路完全相同。

## 数据路径

接收：Driver callback/ISR → transport RX buffer → protocol parser → session dispatch → application event。

发送：Application command → serializer/framer → session → transport send queue → Driver/Stack。
