from __future__ import annotations

from pathlib import Path
import re
import shutil
import sys

try:
    import yaml  # type: ignore
except Exception:
    yaml = None

from .frontmatter import parse_front_matter, require_keys
from .markdown import markdown_to_html, markdown_to_plain_text, rewrite_markdown_image_links
from .models import ExplanationPage, KnowledgePage
from .navigation import classify_page, explanation_filename, knowledge_filename

def validate_project_euler_page(page: ExplanationPage) -> None:
    if classify_page(page) != "Project Euler（100番まで）":
        return
    m = re.search(r"\d+", str(page.problem))
    if not m:
        raise ValueError(f"{page.source_path}: Project Euler は problem に 1 以上 100 以下の番号を含めてください")
    number = int(m.group(0))
    if not (1 <= number <= 100):
        raise ValueError(f"{page.source_path}: Project Euler は100番までのみ掲載対象です")


ALLOWED_EXPLANATION_FILE_SUFFIXES = {".md", ".cpp", ".png"}


def warn_unexpected_explanation_files(src_dir: Path) -> None:
    """解説ディレクトリ内の想定外ファイルを警告する。ビルド自体は継続する。"""
    if not src_dir.exists():
        return
    for path in sorted(src_dir.rglob("*")):
        if not path.is_file():
            continue
        if path.suffix.lower() in ALLOWED_EXPLANATION_FILE_SUFFIXES:
            continue
        print(
            f"WARNING: explanations 配下に .md / .cpp / .png 以外のファイルがあります: {path}",
            file=sys.stderr,
        )


def is_source_file_format_error(exc: BaseException) -> bool:
    """入力Markdown 1ファイルの形式不正として扱ってよい例外か判定する。"""
    if isinstance(exc, (OSError, UnicodeError, ValueError, TypeError, KeyError)):
        return True
    return yaml is not None and isinstance(exc, yaml.YAMLError)


def warn_skipped_source_file(kind: str, path: Path, exc: BaseException) -> None:
    detail = str(exc)
    path_prefix = f"{path}: "
    if detail.startswith(path_prefix):
        detail = detail[len(path_prefix) :]
    print(
        f"WARNING: 形式が不正な{kind}をスキップします: {path}: {detail}",
        file=sys.stderr,
    )


def load_explanations(src_dir: Path) -> list[ExplanationPage]:
    pages = []
    for path in sorted(src_dir.glob("**/*.md")):
        try:
            meta, body_md = parse_front_matter(path.read_text(encoding="utf-8"), path)
            require_keys(meta, ["contest", "problem", "problem_title", "problem_url", "submission_url", "tags"], path)
            body_md_for_html, assets = rewrite_markdown_image_links(
                body_md,
                source_path=path,
                source_root=src_dir,
                output_subdir="explanations",
            )
            body_html = markdown_to_html(body_md_for_html, source_path=path)
            body_text = markdown_to_plain_text(body_md)
            url = f"explanations/{explanation_filename(str(meta['contest']), str(meta['problem']))}"
            page = ExplanationPage(path, meta, body_md, body_html, body_text, url, assets)

            # 後段で初めて例外にならないよう、形式依存の項目は読み込み時に検証する。
            page.tags
            validate_project_euler_page(page)
        except Exception as exc:
            if not is_source_file_format_error(exc):
                raise
            warn_skipped_source_file("問題解説", path, exc)
            continue
        pages.append(page)
    return pages


def load_knowledge(src_dir: Path) -> list[KnowledgePage]:
    pages = []
    if not src_dir.exists():
        return pages
    for path in sorted(src_dir.glob("**/*.md")):
        try:
            meta, body_md = parse_front_matter(path.read_text(encoding="utf-8"), path)
            require_keys(meta, ["title"], path)
            body_md_for_html, assets = rewrite_markdown_image_links(
                body_md,
                source_path=path,
                source_root=src_dir,
                output_subdir="knowledge",
            )
            body_html = markdown_to_html(body_md_for_html, source_path=path)
            body_text = markdown_to_plain_text(body_md)
            url = f"knowledge/{knowledge_filename(path)}"
            page = KnowledgePage(path, meta, body_md, body_html, body_text, url, assets)

            # 一覧生成・関連知識解決で使う形式依存項目を先に検証する。
            page.aliases
            page.absorbs
            page.related
            page.category_order
            page.level_order
        except Exception as exc:
            if not is_source_file_format_error(exc):
                raise
            warn_skipped_source_file("知識記事", path, exc)
            continue
        pages.append(page)
    return pages


def clean_output(out_dir: Path) -> None:
    out_dir.mkdir(parents=True, exist_ok=True)
    for child in out_dir.iterdir():
        if child.is_dir():
            shutil.rmtree(child)
        else:
            child.unlink()


def copy_page_assets(pages: list[ExplanationPage] | list[KnowledgePage], out_dir: Path) -> None:
    copied: set[tuple[Path, Path]] = set()
    for page in pages:
        for asset in page.assets:
            if not asset.source_path.exists():
                print(f"WARNING: 画像ファイルが見つかりません: {page.source_path}: {asset.source_path}", file=sys.stderr)
                continue
            dst = out_dir / asset.output_path
            key = (asset.source_path, dst)
            if key in copied:
                continue
            dst.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(asset.source_path, dst)
            copied.add(key)
