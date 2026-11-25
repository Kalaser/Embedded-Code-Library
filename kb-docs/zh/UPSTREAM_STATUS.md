# 上游平台版本状态快照

**核查日期：2026-11-25**

本页只记录本次整理时从官方来源确认到的版本线索，不能替代项目开始时重新核对 release、manifest 和目标板支持情况。产品工程必须固定具体 release/tag/commit。

| 项目 | 本次核查到的版本状态 | 官方来源 | 说明 |
|---|---|---|---|
| Apache NuttX | 13.1.1，发布日期 2026-11-13 | https://nuttx.apache.org/download/ | OS 与 Apps 分开发布，需配套选择版本 |
| Zephyr | 4.5.0，发布日期 2026-10-28；本次查到的最新稳定版 | https://github.com/zephyrproject-rtos/zephyr/blob/main/doc/releases/index.rst | 4.4 与 3.7 LTS 等历史稳定分支仍需按支持周期选择 |
| Yocto Project | 6.1 Blacksail（2026-09 稳定发布）；6.0 Wrynose 为 LTS，支持至 2030-04 | https://www.yoctoproject.org/development/releases/ | 新 layer 的 `LAYERSERIES_COMPAT` 必须匹配实际目标 release |
| RT-Thread | 本次上游 Releases 页面显示 v5.3.0 | https://github.com/RT-Thread/rt-thread/releases | 需按芯片 BSP 和组件包确认适配，不应仅凭内核版本判断 |
| ESP-IDF | 本次访问到的稳定文档版本为 6.1.1 | https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/build-system.html | 目标芯片是否支持该版本需按官方支持矩阵确认 |
| FreeRTOS | 使用 Kernel 与 Distribution 两个上游仓库 | https://github.com/FreeRTOS/FreeRTOS | 选择 Kernel tag，并核对 Port 与 Demo 依赖 |
| Buildroot | 官方手册滚动维护 | https://buildroot.org/downloads/manual/manual.html | 固定 release/commit；external tree 与该版本配套验证 |

## 使用规则

1. 不要将 `main`、`master`、`latest` 当作产品依赖版本。
2. 每个产品工程记录完整 commit SHA、SDK 版本、工具链版本和 submodule revision。
3. NuttX 的 `nuttx` 与 `nuttx-apps` 必须选择兼容版本。
4. Zephyr workspace 应固定 manifest revision，而不只固定应用仓库。
5. Yocto/Buildroot 产品发行版应固定整个构建元数据仓库集合。
