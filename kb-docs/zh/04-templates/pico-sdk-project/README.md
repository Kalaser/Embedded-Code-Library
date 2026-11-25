# Raspberry Pi Pico SDK Project Template

## 前置条件

- 下载并固定版本 Raspberry Pi Pico SDK。
- 设置 `PICO_SDK_PATH` 或使用 submodule/workspace 管理 SDK。
- 当前模板假设 SDK 自带 `pico_sdk_import.cmake` 可从 `PICO_SDK_PATH` 找到。

## 构建

```bash
cmake -S . -B build -DPICO_SDK_PATH=/path/to/pico-sdk
cmake --build build
```

生成 UF2 等额外输出依赖目标板和 SDK 配置。
