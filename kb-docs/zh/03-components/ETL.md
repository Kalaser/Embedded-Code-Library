# Embedded Template Library (ETL)

- 上游：https://github.com/ETLCPP/etl
- 定位：面向嵌入式 C++ 的固定容量容器、算法和工具库。

## 值得借鉴

- 编译期固定容量容器，便于预算 RAM。
- 接近 STL 的接口但不追求替代完整 STL。
- message、variant、FSM、algorithm 等嵌入式工具。
- 容量和错误处理策略显式。

## 适用场景

需要 C++，但不希望在关键路径依赖无界动态分配的产品；固定大小队列、消息载荷、状态机和静态对象集合。

## 接入前确认

- 所选 ETL release 支持的 C++ 标准和编译器。
- `etl_profile.h` 与项目错误处理/异常策略。
- 固定容量溢出时的行为。
- 对象大小、拷贝成本和栈占用。

ETL 不应被描述为完整 STL 替代品；按需使用具体容器。
