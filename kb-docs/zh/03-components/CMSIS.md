# Arm CMSIS

- 上游：https://github.com/ARM-software/CMSIS_6
- 文档：https://arm-software.github.io/CMSIS_6/latest/

CMSIS 是 Arm Cortex-M/A/R 生态的一组软件接口、头文件、工具和设备支持机制。不同 CMSIS 子项目承担不同职责，例如 Core、Device、DSP、RTOS API、Driver API、Pack 等。项目应按目标内核和所用 CMSIS 版本选择对应子项目。

## 重点研究

- CMSIS-Core：核心寄存器、异常/中断、内建函数。
- CMSIS-Device：SoC/MCU 设备定义和外设寄存器。
- CMSIS-DSP：信号处理函数库。
- CMSIS-RTOS2：跨 RTOS 风格的内核 API 抽象。
- CMSIS-Pack：软件组件与设备支持包元数据。

CMSIS-RTOS2 是接口抽象，不意味着任意 RTOS 都具备完全相同的扩展能力或调度行为。移植时仍需检查目标 OS 的实现和限制。
