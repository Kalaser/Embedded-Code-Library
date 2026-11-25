# Third-party Components

当前目录不预置第三方源码。上游项目先登记在 `catalog/components.json`，并保持 `catalog-only` 状态。

完成许可证审查、固定版本、依赖梳理和构建验证后，才可在本目录建立具体组件目录。

建议目录：

```text
third_party/<component>/
├── upstream/             # 可选：submodule 或源码镜像
├── patches/              # 本地补丁
├── integration/          # 本地构建/配置适配
├── THIRD_PARTY_NOTICES.md
└── README.md
```

如果使用 Git Submodule，`upstream/` 不应被当作普通源码副本管理。
