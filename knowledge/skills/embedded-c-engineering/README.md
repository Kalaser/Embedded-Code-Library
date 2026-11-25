# Embedded C Engineering Skill

**Version:** 1.0.0  
**Language:** Chinese (zh-CN)  
**Purpose:** 面向嵌入式、RTOS、驱动、协议栈和系统软件开发的 C 语言工程技能包。

## Package Layout

```text
embedded-c-engineering-skill/
├── SKILL.md
├── README.md
├── CHANGELOG.md
├── references/
│   ├── c-coding-standard.md
│   ├── c-control-flow-and-state-machine.md
│   └── c-data-structures-and-memory-layout.md
├── templates/
│   ├── code-review-checklist.md
│   └── engineering-rule-template.md
└── scripts/
    └── validate_skill.mjs
```

## Source File Renaming

本次重构后的名称与原始资料对应如下：

| 原始资料 | Skill 内名称 | 调整方向 |
|---|---|---|
| `coding_standard.md` | `references/c-coding-standard.md` | 正式编码规范，明确 MUST/SHOULD/MAY、适用边界与审查要求 |
| `loop_branch_techniques.md` | `references/c-control-flow-and-state-machine.md` | 控制流与状态机工程指南，弱化“技巧炫技”，强化边界和验证 |
| `struct_union_techniques.md` | `references/c-data-structures-and-memory-layout.md` | 数据结构与内存布局指南，强化标准语义、ABI、协议解析和硬件风险 |

原始内容进行了重新组织和技术表述修订，不是逐字复制。旧文件名不再作为 Skill 内的正式引用名称。

## Usage

1. 将 `embedded-c-engineering-skill` 目录放入 Agent/Skills 能识别的 skills 目录。
2. 确认入口文件名为 `SKILL.md`。
3. 按任务需要读取 `references/` 中的资料；不需要一次加载全部文档。
4. 可运行 `node scripts/validate_skill.mjs` 检查目录、元数据、引用文件和 Markdown 代码围栏。

## Engineering Scope

本 Skill 是工程指导资料，不替代 ISO C 标准、编译器 ABI 文档、芯片手册、RTOS 官方文档或安全认证要求。项目应明确采用的 C 语言版本、编译器版本和目标平台约束。

## Design Principles

- 编码规范、控制流指南和数据结构指南职责分离。
- 将标准保证、实现定义行为、平台扩展和项目约定明确区分。
- 代码示例优先保证可读性、边界安全和可测试性，不以“技巧性”代替工程价值。
- 对未在目标工具链验证的内容明确标注。
