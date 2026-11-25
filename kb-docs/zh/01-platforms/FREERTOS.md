# FreeRTOS 工程体系

## 定位

FreeRTOS 是面向微控制器的实时内核。典型应用由 Kernel、CPU Port、FreeRTOSConfig.h、芯片/BSP、驱动和应用任务组成。FreeRTOS 官方 distribution 仓库还包含预配置 Demo 和其他库；Kernel 仓库是内核本体，二者不要混淆。

## 典型结构

```text
firmware/
├── CMakeLists.txt / vendor IDE project
├── config/FreeRTOSConfig.h
├── platform/                 # startup、clock、interrupt、BSP
├── drivers/
├── middleware/
├── app/
│   ├── app_main.c
│   ├── tasks/
│   ├── services/
│   └── queues/
└── third_party/FreeRTOS-Kernel/  # submodule 或固定版本依赖
```

## 核心对象

- Task：独立执行上下文，需定义优先级、栈和生命周期。
- Queue：任务间传递消息，提供数据所有权边界。
- Semaphore：同步事件或计数资源；二值信号量常用于 ISR 到任务通知。
- Mutex：互斥访问共享资源；需考虑优先级继承和锁顺序。
- Software Timer：由 Timer Service Task 执行回调，回调不应长时间阻塞。
- Event Groups / Task Notifications：适用于事件位或轻量通知。

## 典型运行模型

1. 硬件和平台初始化。
2. 初始化应用服务和通信驱动。
3. 创建任务、队列、信号量、软件定时器。
4. 启动调度器。
5. 任务按优先级执行，通过队列/通知协作。

## 关键配置

`FreeRTOSConfig.h` 与具体 Port 强相关。重点检查时钟节拍、最大优先级、最大任务数、堆方案、中断优先级边界、软件定时器、任务通知、统计与运行时追踪等设置。

## 常见工程风险

- 任务栈大小凭经验设置，未读取高水位。
- ISR 调用非 FromISR API。
- 在高优先级任务中执行长时间阻塞。
- Mutex 与 Semaphore 混用导致所有权不清。
- 队列传递大结构体造成不必要复制，或传递指针但没有定义所有权。
- 误以为启用 RTOS 就自然获得线程安全。

## 推荐研究入口

- FreeRTOS Kernel: https://github.com/FreeRTOS/FreeRTOS-Kernel
- FreeRTOS Distribution / Demos: https://github.com/FreeRTOS/FreeRTOS
- Kernel Book: https://www.freertos.org/Documentation/02-Kernel/00-Overview
