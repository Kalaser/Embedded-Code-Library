# LittleFS

- 上游：https://github.com/littlefs-project/littlefs
- 设计文档：https://github.com/littlefs-project/littlefs/blob/master/DESIGN.md
- 定位：面向微控制器 Flash 的小型、掉电恢复能力较强的文件系统。

## 设计思想

- Block Device 接口隔离 Flash 硬件。
- Metadata Pair 与日志式元数据更新。
- Copy-on-Write 数据结构。
- 动态 wear leveling 与坏块处理能力。
- RAM 使用具有明确边界，可由调用方提供配置缓冲。

## 移植接口

实现 read、prog、erase、sync 等 block device 回调，并提供 read/prog cache、lookahead buffer 等配置。具体接口以固定版本头文件和文档为准。

## 接入前检查

- Flash 擦写粒度、读写对齐和 page size。
- 擦除块大小、总块数和坏块策略。
- 掉电期间正在执行的操作及恢复测试。
- 多线程访问是否通过单一 owner 或锁序列化。
- 格式化、挂载失败和空间不足时的恢复策略。

LittleFS 的具体持久化保证和限制应以所选版本的 DESIGN.md / SPEC.md 为准，不要将它描述为对所有底层 Flash 故障均可恢复。
