# POSIX System Programming Skill

这是一个目录化的 Agent Skill 包。入口文件为 `SKILL.md`，配套工程资料存放在 `references/`。

## 目录结构

```text
posix-system-programming-skill/
├── SKILL.md
├── README.md
└── references/
    ├── coding_standard.md
    ├── loop_branch_techniques.md
    ├── nuttx_api.md
    └── struct_union_techniques.md
```

## 安装/使用

将整个 `posix-system-programming-skill` 目录放入 Agent 支持的 skills 目录，并确保目录名与 Skill 名称对应。Agent 应以 `SKILL.md` 的 frontmatter `name` 和 `description` 识别技能。

## 加载策略

先读取 `SKILL.md`。只有任务需要时，才读取 `references/` 中对应的资料：

- C 代码风格、安全、错误处理或并发审查：`coding_standard.md`
- NuttX 接口、任务模型和配置：`nuttx_api.md`
- 状态机、循环和分支：`loop_branch_techniques.md`
- C 结构体、联合体和消息载荷：`struct_union_techniques.md`

参考资料保留上传时的原始内容。它们是工程知识，不替代 POSIX 标准或目标操作系统的当前官方文档。
