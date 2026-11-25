# Device Manager 与 Session Manager

## 为什么分开

Device Manager 管理“设备是谁、在哪里、是否在线”；Session Manager 管理“某个连接/业务会话当前处于什么状态”。设备身份与连接句柄不能混为一谈。

## 逻辑模型

```text
Device Registry
  device_id -> Device Record
                  |
                  +-- capabilities
                  +-- firmware / hardware info
                  +-- transport candidates
                  +-- online / offline state
                  |
                  v
             Session Manager
                  |
                  +-- session_id
                  +-- transport handle
                  +-- connection state
                  +-- timeout / heartbeat
                  +-- pending requests
```

## Device Record 建议字段

- 稳定设备标识：SN、设备证书 ID 或产品定义 ID。
- 设备类型和硬件版本。
- 固件版本和能力集合。
- 当前可用的传输端点。
- 最近一次在线时间和状态。
- 关联 Session 的引用或 ID。

## 关键规则

- IP、MAC、UART 端口号和 file descriptor 都可能变化，不能单独当作永久设备身份。
- 连接重建时必须防止旧 Session 的迟到消息污染新 Session。
- 每个 Session 应有 generation/token，异步回调携带会话标识。
- 心跳超时不等同于永久设备丢失；应允许重连和状态恢复。
- 多台设备通过 AP/Wi-Fi 连接时，以 SN/认证身份绑定，不能仅按 DHCP 地址绑定。
- pending command 必须有超时、取消、重试和重复响应策略。

## 适用项目

工厂测试 PC 管理多台设备、IoT 网关、设备维护工具、USB 多设备控制器和无线设备管理系统。
