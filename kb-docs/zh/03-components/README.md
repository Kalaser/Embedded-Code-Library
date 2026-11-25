# 可复用中间件与框架

这里收录的是“应优先评估是否直接复用”的成熟上游项目，不是要求把所有项目都集成进每个固件。

| 项目 | 领域 | 适用平台/场景 | 重点学习 |
|---|---|---|---|
| TinyUSB | USB Device/Host | 裸机、FreeRTOS、RT-Thread 等 | Class Driver、DCD/HCD、OSAL、缓冲区 |
| lwIP | TCP/IP | MCU + Ethernet/Wi-Fi | netif、pbuf、tcp/udp、port 层 |
| Nanopb | 序列化 | 资源受限 C 设备 | `.proto`、静态字段、编解码边界 |
| LittleFS | Flash 文件系统 | NOR/NAND block device adapter | COW、metadata pair、wear leveling |
| LVGL | GUI | MCU/MPU、裸机/RTOS/Linux | display/input port、tick、OSAL、widget |
| ETL | C++ 容器/工具 | 无/少动态分配环境 | fixed-capacity containers、FSM、消息 |
| MCUboot | Bootloader | 多种 MCU/RTOS/SoC | image format、bootutil、swap/recovery |
| Unity/Ceedling | 测试 | Host + Embedded C | test runner、mock、target/host 分离 |
| CMSIS | Arm 软件接口 | Cortex-M/A 生态 | Core、Device、RTOS API、Pack |

每个项目接入前必须检查目标版本、许可证、平台支持、依赖、内存开销和产品限制。
