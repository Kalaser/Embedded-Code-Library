import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';

const root = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const configPath = path.join(root, 'docmd.config.json');
const config = JSON.parse(fs.readFileSync(configPath, 'utf8'));
const sourceRoot = path.resolve(root, config.src ?? 'docs');
const locale = config.i18n?.default;
const contentRoot = locale ? path.join(sourceRoot, locale) : sourceRoot;
const errors = [];

function walkNavigation(items = []) {
  for (const item of items) {
    if (item.path) {
      const route = item.path.replace(/^\/+|\/+$/g, '');
      const base = route ? path.join(contentRoot, route) : contentRoot;
      const candidates = route
        ? [`${base}.md`, path.join(base, 'index.md'), path.join(base, 'README.md')]
        : [path.join(contentRoot, 'index.md'), path.join(contentRoot, 'README.md')];
      if (!candidates.some((target) => fs.existsSync(target))) {
        errors.push(`Navigation target not found: ${item.path} -> ${candidates.map((target) => path.relative(root, target)).join(' | ')}`);
      }
    }
    if (item.children) walkNavigation(item.children);
  }
}

function resolveMarkdownTarget(sourceFile, rawTarget) {
  const clean = rawTarget.split('#')[0].split('?')[0];
  if (!clean) return true;
  const candidate = path.resolve(path.dirname(sourceFile), decodeURIComponent(clean));
  const candidates = [candidate, `${candidate}.md`, path.join(candidate, 'index.md'), path.join(candidate, 'README.md')];
  return candidates.some((item) => fs.existsSync(item));
}

function walkMarkdown(directory) {
  for (const entry of fs.readdirSync(directory, { withFileTypes: true })) {
    const current = path.join(directory, entry.name);
    if (entry.isDirectory()) {
      walkMarkdown(current);
      continue;
    }
    if (!entry.isFile() || !entry.name.toLowerCase().endsWith('.md')) continue;
    const content = fs.readFileSync(current, 'utf8');
    const links = content.matchAll(/!?\[[^\]]*\]\(([^)]+)\)/g);
    for (const match of links) {
      const raw = match[1].trim().split(/\s+/)[0].replace(/^<|>$/g, '');
      if (/^(?:[a-z]+:|\/|#)/i.test(raw)) continue;
      if (!resolveMarkdownTarget(current, raw)) {
        errors.push(`Broken relative link: ${path.relative(root, current)} -> ${raw}`);
      }
    }
  }
}

if (!fs.existsSync(contentRoot)) {
  errors.push(`Content directory not found: ${path.relative(root, contentRoot)}`);
} else {
  walkNavigation(config.navigation ?? []);
  walkMarkdown(contentRoot);
}

if (errors.length) {
  console.error(`[check-site] ${errors.length} issue(s) found:`);
  for (const error of errors) console.error(`- ${error}`);
  process.exit(1);
}

console.log(`[check-site] OK: navigation and relative Markdown links verified in ${path.relative(root, contentRoot)}.`);
