from __future__ import annotations

from pathlib import Path

from .config import TABLE_CELL_BACKGROUNDS

STYLE_TEMPLATE = Path(__file__).with_name("static") / "style.css.template"


def write_css(out_dir: Path) -> None:
    table_background_variables = "\n".join(
        f"  --table-bg-{name}: {value};" for name, value in TABLE_CELL_BACKGROUNDS.items()
    )
    table_background_rules = "\n".join(
        f".markdown-body .table-bg-{name} {{ background-color: var(--table-bg-{name}); }}"
        for name in TABLE_CELL_BACKGROUNDS
    )
    css = STYLE_TEMPLATE.read_text(encoding="utf-8")
    css = css.replace("__TABLE_BACKGROUND_VARIABLES__", table_background_variables)
    css = css.replace("__TABLE_BACKGROUND_RULES__", table_background_rules)
    (out_dir / "style.css").write_text(css, encoding="utf-8")
