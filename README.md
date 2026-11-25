# Embedded Framework Knowledge Base — docmd Website

基于 [docmd](https://github.com/docmd-io/docmd) 的静态技术知识库网站。本站只收录嵌入式平台、优秀框架、架构思想、可复用中间件、工程模板和上游源码参考。

## 本地预览

要求：Node.js 20+、npm。首次执行时，npx 会下载固定版本的 `@docmd/core@0.9.8`。

```bash
npm run dev
```

打开 `http://127.0.0.1:3000`。

## 验证与构建

```bash
npm run check:site
npm run validate
npm run build
```

生成目录：`site/`。docmd 是静态站点生成器，构建产物可部署到 GitHub Pages 或其他静态托管平台。

## 内容维护

- `kb-docs/zh/`：站点实际发布的 Markdown 内容源，可以直接编辑。
- `assets/css/`：站点样式。
- `assets/downloads/`：完整知识库及多平台工程模板下载包。
- `docmd.config.json`：站点元数据、导航、语言和 Pages 子路径配置。
- `.github/workflows/docs.yml`：构建、链接验证和 GitHub Pages 部署。

更新知识库内容时，先修改 `kb-docs/zh/` 对应页面；如同时更新可下载的完整资料包，也需重新生成 ZIP/tar.gz 并同步更新版本号和下载链接。

## GitHub Pages

本项目仓库 `Kalaser/kalaser_docmd` 的预期项目站点地址：

`https://kalaser.github.io/kalaser_docmd/`

推送到 `main` 后，GitHub Actions 会运行 docmd 链接校验与构建；成功后部署到 GitHub Pages。

首次启用时，在仓库 **Settings → Pages → Build and deployment → Source** 选择 **GitHub Actions**。

> 注意：当前目标仓库是 Private。GitHub Pages 能否从私有仓库发布，取决于仓库所有者的 GitHub 计划。若为 GitHub Free，仓库需设为 Public；公开前必须检查仓库是否含有不应公开的内容。GitHub Pages 发布的网站默认可能对互联网公开，即使源仓库为 Private，也必须单独核对站点可见性设置。

## 目录结构

- `kb-docs/zh/`：中文文档源，按平台、架构、组件、模板及参考项目分组。
- `assets/css/`：自定义 CSS。
- `assets/downloads/`：完整知识库和模板 ZIP / tar.gz 归档。
- `.github/workflows/docs.yml`：GitHub Actions 自动化发布。

原仓库中的 `docs/` 目录暂时保留，但不作为本站内容源。
