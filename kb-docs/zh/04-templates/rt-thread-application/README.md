# RT-Thread Application Template

将 `main.c` 放入目标 RT-Thread BSP 的 `applications/`（或该 BSP 约定的应用目录），并确保其 SCons 工程包含该目录。RT-Thread BSP 的自动发现和构建方式可能有差异，按目标 BSP 的 `SConscript`/`SConstruct` 和 README 集成。

## 入口

很多 RT-Thread BSP 使用应用 `main()` 作为主线程入口；不要在同一工程中再创建冲突的 `main()`。应用任务也可通过线程创建 API 启动，取决于产品架构。
