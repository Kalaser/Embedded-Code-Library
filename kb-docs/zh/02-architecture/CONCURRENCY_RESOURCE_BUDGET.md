# 并发模型与资源预算

## 并发模型选择

| 模型 | 优点 | 代价 | 适用场景 |
|---|---|---|---|
| Super Loop | 简单、资源少、执行顺序直观 | 单次执行时间影响整体响应 | 小型无 RTOS 产品 |
| Cooperative Tasks | 显式让出执行权 | 忘记 yield 会阻塞系统 | 简单协作式系统 |
| Preemptive RTOS | 响应和任务隔离更灵活 | 锁、优先级、栈和调度复杂 | 多任务产品 |
| Event Loop | 线程少、状态集中 | 回调和事件队列设计要求高 | I/O 密集、连接管理 |
| Hybrid | 将硬实时任务与事件服务分开 | 架构边界和通信更多 | 复杂产品 |

## 必须量化的资源

- 每个 Task/Thread 的栈大小和高水位。
- Queue 长度、单条消息大小及最大积压时间。
- 静态缓冲区、DMA buffer、协议帧最大长度。
- Heap 分配次数、碎片风险和失败策略。
- 中断执行时间、关中断区间和最大响应时间。
- CPU 使用率、调度抖动和 watchdog 窗口。
- Cache/DMA 一致性、内存对齐和可访问内存区域。

## 队列背压

消息队列必须明确满队列策略：阻塞、丢弃最新、丢弃最旧、覆盖、流控或上游限速。选择哪一种取决于事件是否可丢失以及实时要求。

## 共享数据

优先用消息传递减少共享状态；必须共享时定义互斥策略、锁顺序和生命周期。不要把 `volatile` 当作线程同步机制。

## 资源预算模板

| 资源 | 预算 | 实测 | 峰值/水位 | 结论 |
|---|---:|---:|---:|---|
| Main stack | TBD | TBD | TBD | TBD |
| Task A stack | TBD | TBD | TBD | TBD |
| RX queue | TBD | TBD | TBD | TBD |
| TX buffers | TBD | TBD | TBD | TBD |
| Heap | TBD | TBD | TBD | TBD |
| DMA buffers | TBD | TBD | TBD | TBD |
