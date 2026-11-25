# 分层固件架构

## 参考层次

```text
Application
    |
Product Services
    |
Middleware / Framework
    |
OS Adaptation (optional)
    |
RTOS / Runtime
    |
HAL / Drivers
    |
BSP / SoC
    |
Hardware
```

## 层职责

- Application：产品用例和业务流程，不直接操作寄存器。
- Product Services：设备管理、会话、配置、日志、升级等产品服务。
- Middleware：协议栈、文件系统、GUI、序列化等可复用功能。
- OS Adaptation：当需要支持多个 OS 时，集中封装线程、锁、队列、定时器等真正变化的原语。
- RTOS/Runtime：任务调度、同步和系统服务。
- HAL/Drivers：外设功能和硬件访问。
- BSP/SoC：板卡差异、时钟、引脚、中断和内存映射。

## 依赖原则

默认上层依赖下层定义的接口。底层不能 include 产品业务层。跨层事件应通过接口、回调或消息传递，不应通过共享全局变量穿透层次。

## 何时不需要某一层

小项目可以把 Product Services 与 Application 合并；单 OS 产品不一定需要通用 OSAL；简单芯片也可以由 HAL 与 BSP 合并。只有当边界能减少变化传播和重复代码时才增加层。

## 代码审查问题

1. 修改某个 UART 驱动是否必须改业务逻辑？
2. 把业务模块移到另一 RTOS 是否需要修改业务状态机？
3. 上层是否依赖了 BSP 私有头文件？
4. 接口是否表达资源所有权、错误码和阻塞行为？
5. 是否出现循环依赖或跨层访问全局变量？
