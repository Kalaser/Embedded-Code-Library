# Bootloader、固件更新与故障恢复

## 启动链路

```text
ROM / First Stage Boot
         |
         v
Bootloader
  - image validation
  - version / security policy
  - slot selection
  - rollback / recovery
         |
         v
Application Firmware
  - startup / BSP
  - self-test
  - confirm image (if required)
```

## 双镜像/交换式升级常见概念

- Primary / Secondary Slot：当前运行镜像和候选镜像。
- Image Header：版本、长度、标识和安全元数据。
- Signature Verification：启动前验证签名。
- Swap/Overwrite/Direct-XIP：不同升级策略，Flash 分区和恢复能力不同。
- Pending/Confirmed：新镜像试运行和确认状态。
- Rollback：启动失败或未确认时恢复旧版本。

## 设计要求

- Flash 分区表必须与 Bootloader、应用 Linker Script 一致。
- 镜像下载完成后先校验长度、哈希、签名和兼容硬件版本。
- 断电注入测试要覆盖擦除、写入、状态标记和镜像切换窗口。
- 防回滚策略需要与产品安全策略协调。
- Bootloader 与应用的共享元数据格式需版本化。
- Recovery 模式要有明确入口和恢复工具。

## 参考项目

MCUboot 将通用 bootutil 功能与各平台 boot application 分离，便于复用和单元测试。官方文档：https://docs.mcuboot.com/ 。
