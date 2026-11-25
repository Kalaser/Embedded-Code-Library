# 事件驱动与状态机

## 适用场景

异步通信、连接管理、协议解析、传感器采集、设备测试流程、升级过程和 UI 控制流程。

## 推荐结构

```text
ISR / Driver callback / Timer / Network callback
                    |
                    v
              Event Adapter
                    |
                    v
              Event Queue
                    |
                    v
               Event Loop
                    |
                    v
             State Machine
                    |
          +---------+---------+
          |                   |
      Service API         Transport API
```

## 设计区分

- Event：描述已经发生的事实，如 RX_READY、LINK_DOWN、TIMEOUT。
- Command：请求系统执行动作，如 CONNECT、START_TEST、RESET_DEVICE。
- State：当前业务阶段，如 IDLE、CONNECTING、ACTIVE、ERROR。
- Transition：某个事件在当前状态下触发的状态和动作变化。

不要把所有命令和事件都塞进一个无类型的 `void *` payload；应使用带 tag 的事件结构并定义数据所有权。

## 状态机设计要求

- 状态枚举完整，未识别状态有明确兜底行为。
- 转换条件可追踪，不从多个线程随意修改状态变量。
- 每个等待都有超时或取消策略。
- 错误状态定义恢复、重试、复位或终止策略。
- 资源获取和释放与状态生命周期对应。
- 幂等操作明确，例如重复 STOP、DISCONNECT 是否允许。

## 实现方式

简单流程可使用 `switch(state)`；状态较多且转换关系稳定时使用状态表；并发事件复杂时采用队列驱动的单线程状态机。不要为小型流程引入过度复杂的通用状态机框架。
