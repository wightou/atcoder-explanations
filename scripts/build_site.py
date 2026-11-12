#!/usr/bin/env python3
from __future__ import annotations

import argparse
from pathlib import Path

from site_builder.builder import build
from site_builder.config import DEFAULT_EXPLANATIONS, DEFAULT_KNOWLEDGE, DEFAULT_OUT


def main() -> None:
    parser = argparse.ArgumentParser(description="競プロ解説 Markdown から静的HTMLサイトを生成します。")
    parser.add_argument("--explanations", type=Path, default=DEFAULT_EXPLANATIONS, help="入力解説Markdownディレクトリ")
    parser.add_argument("--knowledge", type=Path, default=DEFAULT_KNOWLEDGE, help="入力知識Markdownディレクトリ")
    parser.add_argument("--out", type=Path, default=DEFAULT_OUT, help="出力ディレクトリ")
    args = parser.parse_args()
    build(args.explanations, args.knowledge, args.out)


if __name__ == "__main__":
    main()
