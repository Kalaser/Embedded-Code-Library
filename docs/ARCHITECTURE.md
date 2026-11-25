# Library Architecture

## 1. Asset categories

- `components/`: 自研、具有稳定接口和独立验证的组件。
- `templates/`: 新工程起始结构，不等同于完整 BSP 或可量产固件。
- `catalog/`: 上游组件发现与选型索引。
- `third_party/`: 经过版本固定和许可证审查后引入的外部代码。
- `knowledge/`: 标准、规范、技能和参考资料。
- `examples/`: 演示如何调用本仓库组件的最小应用。

## 2. Layering model

```text
Product Application
        |
Reusable Services / Domain Components
        |
Portable Component Interfaces
        |
Platform Adapters (OSAL / HAL / BSP)
        |
RTOS / Drivers / MCU / Board
```

依赖只能向下。通用组件不得直接 include 产品业务头文件、板级私有头文件或某个 RTOS 的内部头文件。

## 3. Portability classes

| Class | Definition | Example |
|---|---|---|
| P0 | Pure C/C++ logic, no OS dependency | Ring Buffer, Byte Order |
| P1 | Standard C library dependency | `memcpy`, `snprintf` wrapper |
| P2 | POSIX dependency | `pthread`, `poll`, `mq` |
| P3 | RTOS abstraction dependency | OSAL queue/timer |
| P4 | Platform/BSP dependency | UART DMA, GPIO, sensor bus |

Every component README must declare its portability class.

## 4. Avoid premature abstraction

只有当两个或以上真实目标环境需要同一抽象，且抽象能表达其共同语义时，才引入平台接口。不要为了“未来可能移植”而把简单函数包装成多层虚接口。
