from __future__ import annotations

from contextlib import redirect_stderr, redirect_stdout
from hashlib import sha256
import io
import json
import os
from pathlib import Path
import sys
import tempfile
import unittest
from unittest import mock

ROOT = Path(__file__).resolve().parents[1]
SCRIPTS = ROOT / "scripts"
if str(SCRIPTS) not in sys.path:
    sys.path.insert(0, str(SCRIPTS))

from site_builder import builder, frontmatter, markdown, navigation  # noqa: E402


FIXTURES = Path(__file__).resolve().parent / "fixtures"
EXPLANATIONS = FIXTURES / "explanation_sources"
KNOWLEDGE = FIXTURES / "knowledge_sources"
CONFIG = FIXTURES / "site_config.toml"
SNAPSHOT = Path(__file__).resolve().parent / "expected_site_sha256.json"


def file_manifest(root: Path) -> dict[str, str]:
    manifest: dict[str, str] = {}
    for path in sorted(p for p in root.rglob("*") if p.is_file()):
        rel = path.relative_to(root).as_posix()
        manifest[rel] = sha256(path.read_bytes()).hexdigest()
    return manifest


def format_manifest_diff(expected: dict[str, str], actual: dict[str, str]) -> str:
    expected_names = set(expected)
    actual_names = set(actual)
    missing = sorted(expected_names - actual_names)
    extra = sorted(actual_names - expected_names)
    changed = sorted(
        name
        for name in expected_names & actual_names
        if expected[name] != actual[name]
    )
    parts = []
    if missing:
        parts.append("missing: " + ", ".join(missing))
    if extra:
        parts.append("extra: " + ", ".join(extra))
    if changed:
        parts.append("changed: " + ", ".join(changed))
    return "\n".join(parts) or "no manifest difference"


class SiteBuilderRegressionTest(unittest.TestCase):
    def build_fixture_site(self, out_dir: Path) -> tuple[str, str]:
        contest_views = navigation.load_contest_views(CONFIG)
        stdout = io.StringIO()
        stderr = io.StringIO()
        with (
            mock.patch.object(frontmatter, "yaml", None),
            mock.patch.object(markdown, "markdown_lib", None),
            mock.patch.object(builder, "load_contest_views", return_value=contest_views),
            redirect_stdout(stdout),
            redirect_stderr(stderr),
        ):
            builder.build(EXPLANATIONS, KNOWLEDGE, out_dir)
        return stdout.getvalue(), stderr.getvalue()

    def test_fixture_site_snapshot(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            out_dir = Path(tmp) / "site"
            stdout, stderr = self.build_fixture_site(out_dir)
            actual = file_manifest(out_dir)

        if os.environ.get("UPDATE_SITE_SNAPSHOT") == "1":
            SNAPSHOT.write_text(
                json.dumps(actual, ensure_ascii=False, indent=2, sort_keys=True) + "\n",
                encoding="utf-8",
            )

        expected = json.loads(SNAPSHOT.read_text(encoding="utf-8"))
        self.assertEqual(
            expected,
            actual,
            msg=format_manifest_diff(expected, actual),
        )
        self.assertIn("generated explanations: 2", stdout)
        self.assertIn("generated knowledge pages: 2", stdout)
        self.assertIn("WARNING: 形式が不正な問題解説をスキップします", stderr)
        self.assertIn("broken.md", stderr)
        self.assertIn("WARNING: 形式が不正な知識記事をスキップします", stderr)

    def test_fallback_frontmatter_supports_list_of_dicts(self) -> None:
        source = """\
contest: ABC999
alternative_submission_urls:
  - label: 別解
    url: https://example.com/submissions/2
tags:
  - テスト知識
"""
        with mock.patch.object(frontmatter, "yaml", None):
            parsed = frontmatter.parse_simple_yaml(source)
        self.assertEqual(parsed["contest"], "ABC999")
        self.assertEqual(parsed["tags"], ["テスト知識"])
        self.assertEqual(
            parsed["alternative_submission_urls"],
            [{"label": "別解", "url": "https://example.com/submissions/2"}],
        )

    def test_custom_markdown_syntax_in_fallback_renderer(self) -> None:
        source = """\
{red}赤文字{/red}

`{red}コード{/red}`

{red}閉じ忘れ

<!-- table-row-header: true -->
| 項目 | 値 |
| :--- | ---: |
| {red}赤背景 | {red}表内赤文字{/red} |
"""
        with mock.patch.object(markdown, "markdown_lib", None):
            html = markdown.markdown_to_html(source, source_path=Path("fixture.md"))

        self.assertIn('<span class="text-red">赤文字</span>', html)
        self.assertIn("<code>{red}コード{/red}</code>", html)
        self.assertIn("{red}閉じ忘れ", html)
        self.assertIn("table-bg-red", html)
        self.assertIn('<span class="text-red">表内赤文字</span>', html)
        self.assertIn('scope="row"', html)

    def test_fixture_config_is_readable(self) -> None:
        views = navigation.load_contest_views(CONFIG)
        self.assertEqual(
            [view["id"] for view in views],
            ["ABC", "教育系コンテスト", "その他AtCoder"],
        )
        self.assertEqual(views[0]["columns"], ["A", "B"])
        self.assertEqual(views[1]["groups"], ["EDPC"])


if __name__ == "__main__":
    unittest.main()
