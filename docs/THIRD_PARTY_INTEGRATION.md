# Third-Party Integration Workflow

## 1. Intake

登记组件名称、用途、上游 URL、维护状态、目标版本、使用平台、替代方案和引入理由。

## 2. License review

检查选定 tag/commit 的 LICENSE、NOTICE、SPDX、版权声明、子目录、子模块、示例、BSP、生成文件和传递依赖。记录组织审核结论。未经批准不得标记为可用于产品。

## 3. Version pinning

必须固定 release tag 和 commit SHA。生产构建不得直接跟踪分支头。更新时记录上游变更、API 变化、安全公告、本地补丁冲突和回归结果。

## 4. Integration method

优先级建议：

1. 由平台包管理机制提供且可锁版本的包。
2. Git Submodule 固定 commit。
3. 独立 vendor mirror。
4. 有理由且可追溯的源码摘录。

不要把整个上游仓库无差别复制进 `components/`。

## 5. Required record

在 `third_party/manifest.json` 或组件自己的 `component.json` 记录：上游、版本、commit、许可证、导入日期、本地补丁、依赖、构建方法、测试结果和审查状态。

## 6. Upgrade gate

更新第三方依赖前至少执行：构建、单元测试、集成测试、静态分析（适用时）、目标板验证（适用时）以及许可证差异检查。
