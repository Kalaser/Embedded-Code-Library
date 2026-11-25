# Unity 与 Ceedling

- Unity：https://github.com/ThrowTheSwitch/Unity
- Ceedling：https://github.com/ThrowTheSwitch/Ceedling

Unity 是面向 C 的单元测试框架；Ceedling 提供围绕 C 项目的测试构建、测试运行和 Mock 组织能力。

## 推荐测试分层

- Host Unit Test：验证纯算法、状态机、协议解析和数据结构。
- Mock-based Test：隔离 HAL、OS、Transport 等外部依赖。
- Target Integration Test：验证真实驱动、时序和 RTOS 交互。
- Hardware-in-the-Loop：验证电气接口、真实传感器、射频或功耗行为。

## 关键原则

把业务逻辑尽量写成可以在 Host 上编译的 C 模块；硬件访问通过窄接口隔离。Mock 结果不能代替真实硬件验证。
