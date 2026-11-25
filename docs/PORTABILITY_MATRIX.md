# Portability Matrix

不要只写“支持 NuttX/Linux/FreeRTOS”。必须写明版本、架构、编译器和验证范围。

| Component | Language | C/C++ standard | Host | NuttX | FreeRTOS | Zephyr | Notes |
|---|---|---|---|---|---|---|---|
| ring_buffer | C | C99 | Tested by local CMake | Not yet verified | Not yet verified | Not yet verified | P0; no OS calls; caller serializes |
| byte_order (planned) | C | C99 | Not implemented | N/A | N/A | N/A | Add only when component is implemented |

状态定义：

- `Not applicable`：组件设计上不适用于该平台。
- `Not implemented`：当前无实现。
- `Not yet verified`：理论上可用，但没有足够构建/测试证据。
- `Build verified`：指定环境编译成功。
- `Test verified`：指定环境测试通过。
- `Production used`：明确产品中采用，并记录产品/版本/审批。

Host 测试通过不能自动推导出 RTOS 或 MCU 目标已验证。
