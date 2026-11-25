# Firmware Application Template

这是通用应用层模板，不绑定某个 MCU 或 RTOS。复制前应先明确平台入口、日志接口、时钟、线程模型和配置来源。

建议把业务逻辑与平台启动代码分离：`app/` 放用例逻辑，`platform/` 放 BSP/RTOS 绑定。不要在业务模块直接访问寄存器或调用厂商私有 HAL。
