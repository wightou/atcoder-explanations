from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path
import re

def markdown_needs_mathjax(body_md: str) -> bool:
    """Markdown本文にMathJaxが必要そうな数式区切りがあるか判定する。

    コードブロック・インラインコード内の `$` は数式扱いしない。
    判定は読み込み要否を決めるための軽量なものに留める。
    """
    text = re.sub(r"```.*?```", " ", body_md, flags=re.S)
    text = re.sub(r"`[^`]*`", " ", text)
    return bool(re.search(r"\$\$|(?<!\\)\$(?!\s)|\\\(|\\\[", text))


@dataclass(frozen=True)
class PageAsset:
    source_path: Path
    output_path: Path


@dataclass(frozen=True)
class ExplanationPage:
    source_path: Path
    meta: dict
    body_md: str
    body_html: str
    body_text: str
    url: str
    assets: list[PageAsset]

    @property
    def contest(self) -> str:
        return str(self.meta["contest"])

    @property
    def problem(self) -> str:
        return str(self.meta["problem"])

    @property
    def problem_title(self) -> str:
        return str(self.meta["problem_title"])

    @property
    def problem_title_ja(self) -> str:
        return str(self.meta.get("problem_title_ja", "")).strip()

    @property
    def problem_code(self) -> str:
        return f"{self.contest} {self.problem}"

    @property
    def full_title(self) -> str:
        return f"{self.problem_code} - {self.problem_title}"

    @property
    def full_title_with_ja(self) -> str:
        if self.problem_title_ja:
            return f"{self.full_title}（{self.problem_title_ja}）"
        return self.full_title

    @property
    def tags(self) -> list[str]:
        tags = self.meta.get("tags", [])
        if tags is None:
            return []
        if not isinstance(tags, list):
            raise ValueError(f"{self.source_path}: tags は YAML 配列にしてください")
        return [str(x) for x in tags]

    @property
    def needs_mathjax(self) -> bool:
        return markdown_needs_mathjax(self.body_md)


@dataclass(frozen=True)
class KnowledgePage:
    source_path: Path
    meta: dict
    body_md: str
    body_html: str
    body_text: str
    url: str
    assets: list[PageAsset]

    @property
    def title(self) -> str:
        return str(self.meta["title"])

    @property
    def category(self) -> str:
        return str(self.meta.get("category", "未分類"))

    @property
    def level(self) -> str:
        return str(self.meta.get("level", "未設定"))

    @property
    def summary(self) -> str:
        return str(self.meta.get("summary", "")).strip()

    @property
    def summary_needs_mathjax(self) -> bool:
        return markdown_needs_mathjax(self.summary)

    @property
    def needs_mathjax(self) -> bool:
        return markdown_needs_mathjax(self.body_md)

    def _int_meta(self, key: str, default: int = 9999) -> int:
        raw = self.meta.get(key, default)
        try:
            return int(raw)
        except Exception as exc:
            raise ValueError(f"{self.source_path}: {key} は整数にしてください") from exc

    @property
    def category_order(self) -> int:
        return self._int_meta("category_order")

    @property
    def level_order(self) -> int:
        return self._int_meta("level_order")

    @property
    def aliases(self) -> list[str]:
        aliases = self.meta.get("aliases", [])
        if aliases is None:
            return []
        if not isinstance(aliases, list):
            raise ValueError(f"{self.source_path}: aliases は YAML 配列にしてください")
        return [str(x) for x in aliases]

    @property
    def absorbs(self) -> list[str]:
        absorbs = self.meta.get("absorbs", [])
        if absorbs is None:
            return []
        if not isinstance(absorbs, list):
            raise ValueError(f"{self.source_path}: absorbs は YAML 配列にしてください")
        return [str(x) for x in absorbs]

    @property
    def related(self) -> list[str]:
        related = self.meta.get("related", [])
        if related is None:
            return []
        if not isinstance(related, list):
            raise ValueError(f"{self.source_path}: related は YAML 配列にしてください")
        return [str(x) for x in related]

    @property
    def tag_keys(self) -> list[str]:
        seen: set[str] = set()
        keys: list[str] = []
        for value in [self.title, *self.aliases, *self.absorbs]:
            key = str(value).strip()
            if key and key not in seen:
                seen.add(key)
                keys.append(key)
        return keys
