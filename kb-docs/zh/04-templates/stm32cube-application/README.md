# STM32Cube Application Layer Template

**用途：把产品逻辑从 CubeMX/CubeIDE 生成的 Core 工程中分离。** 不含特定 STM32 系列 HAL、启动文件、时钟配置和 linker script。

## 目录

```text
App/
├── Inc/app.h
└── Src/app.c
```

## 集成

在 CubeMX 生成的 `main.c` 中保留 HAL/Clock/Peripheral 初始化，并在用户代码区域调用：

```c
MX_GPIO_Init();
MX_USARTx_UART_Init();
app_init();
while (1) {
    app_process();
}
```

若工程启用 RTOS，则 `app_process()` 不应长期占用主线程；由产品定义 task/service 入口。
