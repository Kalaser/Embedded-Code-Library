# Versioning and Release

## 1. Repository version

使用语义化版本号：`MAJOR.MINOR.PATCH`。

- MAJOR：目录、组件 API 或发布契约出现不兼容变化。
- MINOR：新增向后兼容组件或能力。
- PATCH：向后兼容的缺陷修复、文档或测试修订。

## 2. Component version

每个可独立发布的组件可拥有自己的版本号。仓库版本不替代组件版本。

## 3. Release gate

- 所有组件均有 README 和组件元数据。
- 测试通过，或明确记录未执行的测试及原因。
- 许可证与第三方清单已更新。
- 不包含未记录的二进制、密钥、凭证或生成产物。
- 变更日志和迁移说明已更新。
