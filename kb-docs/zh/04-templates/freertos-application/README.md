# FreeRTOS Application Template

**前置条件：** 将固定版本 FreeRTOS-Kernel、目标 Port、芯片启动文件、BSP 和有效 `FreeRTOSConfig.h` 集成到实际工程。模板不包含通用可直接使用的 FreeRTOSConfig，因为配置依赖 CPU/Port/中断优先级。

## 启动流程

`main()` → board init → app create objects/tasks → `vTaskStartScheduler()`。

## 重点

- 任务栈按目标 Port 单位配置，并用高水位验证。
- ISR 只能调用适用的 `FromISR` API，并正确处理 yield。
- Timer callback 运行在 Timer Service Task 中，不要阻塞。
- `app_task` 演示使用 `vTaskDelayUntil` 做周期调度；周期精度还受 tick rate 影响。
