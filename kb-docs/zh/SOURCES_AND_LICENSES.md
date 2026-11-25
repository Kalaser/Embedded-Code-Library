# 上游来源与许可证管理

## 来源原则

本仓库保存架构知识、源码阅读入口和自编写模板，不默认镜像或重新发布第三方项目源代码。第三方依赖通过上游仓库、固定 release/commit 和项目自身依赖机制获取。

## 产品集成时必须记录

| 字段 | 要求 |
|---|---|
| Project | 上游项目名 |
| Repository | 官方/可信上游仓库 URL |
| Release | 固定 tag 或版本号 |
| Commit | 实际集成 commit SHA |
| License | 根目录及实际使用子目录的许可证 |
| Notices | NOTICE、版权头和第三方声明 |
| Local patches | 本地补丁路径、原因和维护人 |
| Target platforms | 已验证的 SoC、Board、OS、Compiler |
| Test evidence | 构建日志、测试结果和硬件验证记录 |

## 许可证注意事项

- 仓库根目录许可证不一定覆盖所有子目录、示例、工具和 submodule。
- 芯片厂商 SDK 可能包含不同授权条款、工具限制和再分发限制。
- Buildroot/Yocto 最终镜像会包含大量独立软件包，必须生成并审查软件物料清单（SBOM）与许可证清单。
- MIT/BSD/Apache 等许可证的 NOTICE、版权和再分发条件仍需遵守。
- 本文不是法律意见。产品发布前由组织的法务/开源合规流程确认。

## 推荐依赖方式

1. Git submodule：适用于需要固定源码版本并保留上游历史的依赖。
2. CMake FetchContent：适合 Host 工具或能稳定联网获取的开发环境；量产构建应考虑离线和固定 hash。
3. Vendor SDK / west manifest / ESP-IDF Component Manager：遵循平台官方依赖机制。
4. 手工复制源码：仅用于小型、许可允许、来源和版本可以追踪的组件；必须保留许可证并记录来源。
