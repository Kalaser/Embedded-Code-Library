# Embedded Code Library

**版本：** 1.0.0  
**定位：** 嵌入式 C/C++ 可复用代码、工程模板、第三方组件目录与工程知识库。

Embedded Code Library（ECL）用于沉淀可复用的嵌入式软件资产。它不是某一个 RTOS，也不是第三方代码的简单镜像，而是一个可审计、可移植、可测试、可持续维护的工程资产仓库。

## 1. 设计目标

- **可复用：** 将稳定、边界清楚的功能封装为独立组件。
- **可移植：** 将硬件、RTOS、工具链和产品策略依赖放在明确的适配边界。
- **可验证：** 每个组件都有接口契约、测试和已知限制。
- **可追溯：** 第三方组件记录上游地址、版本、许可证和本地修改。
- **可维护：** 统一目录、命名、版本、变更和废弃策略。

## 2. 仓库边界

本仓库分为四种资产：

| 资产类型 | 目录 | 内容 | 纳入原则 |
|---|---|---|---|
| 自研组件 | `components/` | 可直接编译、测试的 C/C++ 模块 | 代码、测试、文档同时具备 |
| 工程模板 | `templates/` | 新项目/新组件的起始结构 | 说明目标环境和未实现部分 |
| 第三方目录 | `catalog/`、`third_party/` | 外部项目索引和集成记录 | 默认只登记上游，不复制源码 |
| 工程知识 | `knowledge/`、`docs/` | 编码规范、POSIX、NuttX 和架构指南 | 标明来源、版本和适用边界 |

**重要：** 第三方项目清单不等于已集成或已验证。没有 `vendor/` 源码、固定版本、许可证审核和构建测试的项目，只能标记为 `catalog-only`。

## 3. 目录结构

```text
Embedded-Code-Library/
├── README.md
├── CATALOG.md
├── CHANGELOG.md
├── LICENSE_POLICY.md
├── CMakeLists.txt
├── catalog/
│   └── components.json
├── components/
│   ├── common/
│   │   └── include/ecl_status.h
│   └── ring_buffer/
│       ├── include/ecl_ring_buffer.h
│       ├── src/ecl_ring_buffer.c
│       ├── tests/test_ring_buffer.c
│       ├── CMakeLists.txt
│       └── README.md
├── templates/
│   ├── c-component-template/
│   ├── firmware-app-template/
│   ├── nuttx-app-template/
│   └── host-cmake-test-template/
├── docs/
│   ├── ARCHITECTURE.md
│   ├── COMPONENT_CONTRACT.md
│   ├── THIRD_PARTY_INTEGRATION.md
│   ├── PORTABILITY_MATRIX.md
│   ├── VERSIONING_AND_RELEASE.md
│   ├── FACTORY_FRAMEWORK_MAPPING.md
│   └── CONTRIBUTING.md
├── third_party/
│   ├── README.md
│   └── notices/README.md
├── knowledge/
│   ├── README.md
│   ├── skills/embedded-c-engineering/
│   ├── skills/posix-system-programming/
│   └── references/nuttx_api.md
├── examples/
│   └── ring_buffer_demo.c
└── tools/
    ├── validate_library.py
    └── new_component.py
```

## 4. 快速开始

### 4.1 构建并运行自研组件测试

依赖：CMake 3.16+、支持 C99 的 C 编译器（GCC、Clang 或对应工具链）。

```bash
cmake -S . -B build -DECL_BUILD_TESTS=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

Windows PowerShell 同样可以执行上述命令；需要将 CMake 和编译器加入 PATH。

### 4.2 检查仓库结构和组件目录

```bash
python tools/validate_library.py
```

该脚本检查仓库必需目录、组件清单 JSON 和已登记组件的基本元数据，不执行网络检查，也不代表许可证合规审计。

### 4.3 创建新组件骨架

```bash
python tools/new_component.py device_registry --language c --description "Device registry abstraction"
```

命令会在 `components/` 下创建接口、实现、测试占位和 README。生成的代码只是骨架，不能视为已实现功能。

## 5. 组件状态

所有资产应使用以下状态之一：

- `draft`：开发中，接口或实现尚未稳定。
- `experimental`：可试用，尚未完成生产级验证。
- `validated`：已完成规定范围内的构建和测试；必须附验证环境。
- `production`：已在明确产品/平台中采用，并经过项目批准。
- `deprecated`：不再推荐使用，须注明替代方案和移除计划。
- `catalog-only`：仅作为上游项目索引，未复制或验证。

禁止仅因 GitHub 仓库流行或存在示例，就将组件标记为 `validated` 或 `production`。

## 6. 推荐阅读顺序

1. `docs/ARCHITECTURE.md`：理解资产分类及依赖方向。
2. `docs/COMPONENT_CONTRACT.md`：理解组件接口、所有权和错误语义。
3. `CATALOG.md`：选择可复用的上游项目。
4. `docs/THIRD_PARTY_INTEGRATION.md`：完成许可证和版本接入流程。
5. `components/ring_buffer/`：查看首个自研组件的完整示例。
6. `knowledge/`：查阅 C 编码、POSIX 和 NuttX 参考资料。

## 7. 针对 Factory Framework 的使用建议

建议按以下边界复用本仓库资产：

```text
Factory Application
    ├── Test Manager / Test Modules
    ├── Device Manager
    ├── Session Manager
    ├── Protocol / Command Dispatcher
    └── Transport Manager
             |
       Framework Services
    ├── Event / State Machine
    ├── Logging / Configuration
    ├── Buffer / Queue / Memory Pool
    └── OSAL
             |
       Platform Adapters
    ├── NuttX
    ├── Linux Host
    └── Other RTOS
```

此图是建议的逻辑分层，不表示仓库已经提供所有模块。首版实际提供的自研组件以 `components/` 为准。

## 8. 外部项目与许可证

上游仓库只作为参考和集成候选。导入任何源代码前必须：

1. 固定 tag 或 commit SHA，禁止生产构建直接跟踪 `main`/`master`。
2. 保存上游 `LICENSE`、`NOTICE`、版权声明和 SPDX 标识。
3. 检查子目录、示例、BSP、二进制和子模块是否采用不同许可证。
4. 记录本地补丁和来源文件。
5. 完成公司/产品要求的开源许可证审查。

参见 `LICENSE_POLICY.md` 与 `docs/THIRD_PARTY_INTEGRATION.md`。

## 9. 当前版本的边界

v1.0.0 是代码库脚手架，不是完整的生产级固件框架。首版包含一个静态内存、非线程安全的 Ring Buffer 组件和若干工程模板。其它框架及协议组件先登记在目录，后续应按产品需求逐个引入、适配与验证。
