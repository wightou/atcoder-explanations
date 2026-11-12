from __future__ import annotations

from pathlib import Path
import re

try:
    import yaml  # type: ignore
except Exception:
    yaml = None

def parse_simple_yaml(src: str) -> dict:
    if yaml is not None:
        data = yaml.safe_load(src) or {}
        if not isinstance(data, dict):
            raise ValueError("front matter は辞書にしてください")
        return data

    data: dict[str, object] = {}
    current_key: str | None = None
    current_list_item: dict[str, object] | None = None

    for raw in src.splitlines():
        line = raw.rstrip()
        if not line.strip() or line.lstrip().startswith("#"):
            continue

        if line.startswith("  - "):
            if current_key is None:
                raise ValueError("YAML配列の親キーがありません")
            data.setdefault(current_key, [])
            if not isinstance(data[current_key], list):
                raise ValueError(f"{current_key} は配列として読めません")

            item = line[4:].strip()
            m = re.match(r"^([A-Za-z0-9_]+):(?:\s*(.*))?$", item)
            if m:
                current_list_item = {m.group(1): unquote_scalar(m.group(2) or "")}
                data[current_key].append(current_list_item)
            else:
                current_list_item = None
                data[current_key].append(unquote_scalar(item))
            continue

        if line.startswith("    "):
            if current_list_item is None:
                raise ValueError(f"簡易YAMLで読めない行です: {line}")
            m = re.match(r"^\s+([A-Za-z0-9_]+):(?:\s*(.*))?$", line)
            if not m:
                raise ValueError(f"簡易YAMLで読めない行です: {line}")
            current_list_item[m.group(1)] = unquote_scalar(m.group(2) or "")
            continue

        m = re.match(r"^([A-Za-z0-9_]+):(?:\s*(.*))?$", line)
        if not m:
            raise ValueError(f"簡易YAMLで読めない行です: {line}")

        key, value = m.group(1), m.group(2)
        current_key = key
        current_list_item = None
        data[key] = [] if value is None or value == "" else unquote_scalar(value)

    return data


def unquote_scalar(s: str) -> str:
    s = s.strip()
    if len(s) >= 2 and ((s[0] == s[-1] == '"') or (s[0] == s[-1] == "'")):
        return s[1:-1]
    if s == "null":
        return ""
    return s


def parse_front_matter(text: str, path: Path) -> tuple[dict, str]:
    m = re.match(r"\A---\s*\n(.*?)\n---\s*\n?(.*)\Z", text, flags=re.S)
    if not m:
        raise ValueError(f"{path}: front matter がありません")
    return parse_simple_yaml(m.group(1)), m.group(2)


def require_keys(meta: dict, keys: list[str], path: Path) -> None:
    missing = [key for key in keys if key not in meta]
    if missing:
        raise ValueError(f"{path}: 必須項目がありません: {', '.join(missing)}")
