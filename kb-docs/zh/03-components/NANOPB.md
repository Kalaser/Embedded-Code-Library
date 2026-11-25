# Nanopb

- 上游：https://github.com/nanopb/nanopb
- 文档：https://jpa.kapsi.fi/nanopb/docs/
- 定位：面向嵌入式 C 的 Protocol Buffers 实现。

## 使用流程

1. 定义 `.proto` schema。
2. 使用 `protoc` 与 nanopb generator 生成 C 结构体和描述符。
3. 对可变长度字段设置最大容量/静态大小策略。
4. 将 `pb_encode.c`、`pb_decode.c`、`pb_common.c` 等运行时源码集成到项目。
5. 在应用协议中封装消息类型、版本、请求 ID、状态码和认证/校验策略。

## 架构价值

Nanopb 解决数据结构序列化，不负责传输帧、会话、可靠交付、认证或设备发现。推荐分层：

```text
Application Message
      |
Nanopb Encode/Decode
      |
Transport Frame (length/version/sequence/CRC)
      |
UART / USB CDC / TCP / BLE
```

## 主要风险

- 未限制重复字段或字符串最大长度。
- 混淆 protobuf message 与传输帧边界。
- schema 变更未考虑旧固件兼容。
- 不验证解码输入长度和资源限制。
- 生成代码与 nanopb runtime 版本不一致。
