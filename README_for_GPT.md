# GPT作業メモ - 競技プログラミング 知識集積所

このファイルは、ChatGPTにこのリポジトリの構成・生成スクリプト・運用方針を伝えるための作業用メモです。公開サイトのトップに出すREADMEではありません。公開向けREADMEは `README.md` を使います。

## このパッケージの位置づけ

このZIPは、サイト生成スクリプトと設定・仕様書だけを更新するためのメタ用パッケージです。ユーザーが手元で管理している本文や生成済みHTMLを誤って上書きしないよう、次は含めません。

```txt
explanations/
knowledge/
docs/
__pycache__/
```

## 反映手順

リポジトリ直下でZIPを展開したあと、必要に応じて次を実行します。

```bash
python scripts/build_site.py
```

知識記事のスタブを作る場合だけ、次を実行します。

```bash
python scripts/create_knowledge_stubs.py
```

`skip` された既存ファイルは件数だけ表示します。パスだけ確認したい場合は次を使います。本文や front matter の中身は表示しません。

```bash
python scripts/create_knowledge_stubs.py --show-skipped
```

Windows由来の `*:Zone.Identifier` ファイルを消す場合は、リポジトリ直下で次を実行します。

```bash
./scripts/delete_zone_identifier.sh
```

## 現在の構成

- `README.md` は公開向けREADMEです。
- `README_for_GPT.md` はGPT向け作業メモです。
- `knowledge_spec.md` は知識記事Markdownの仕様です。
- `scripts/build_site.py` はサイト生成スクリプトです。
- `scripts/create_knowledge_stubs.py` は未作成の知識記事Markdownスタブを生成する補助スクリプトです。
- `scripts/delete_zone_identifier.sh` はWindows由来の `*:Zone.Identifier` ファイルを削除する補助スクリプトです。

## サイト生成スクリプトの主な仕様

### 問題解説

問題解説Markdownは `explanations/` 配下に置きます。front matter の基本項目は次の通りです。

```yaml
---
contest: ABC462
problem: A
problem_title: "Secret Numbers"
problem_title_ja: "秘密の数"
problem_url: https://atcoder.jp/contests/abc462/tasks/abc462_a
submission_url: https://atcoder.jp/contests/abc462/submissions/...
alternative_submission_urls:
  - label: 別解
    url: https://atcoder.jp/contests/...
tag_note:
tags:
  - string型
  - forループ
---
```


教育系コンテストは、Markdownファイルのディレクトリを別々にし、HTMLでは同じページ内にコンテスト別の一覧として表示します。

```txt
explanations/edpc/a.md
explanations/tdpc/a.md
explanations/ndpc/a.md
explanations/fps24/a.md
```

front matter の `contest` は `EDPC`、`TDPC`、`NDPC`、`FPS24` のように書きます。生成後は `contests/educational.html` に集約され、ページ内で各コンテストごとに一覧を分けます。教育系コンテストページでは、通常コンテスト表の問題セルと同じく、左上に `EDPC B`、中央に `Frog 2`、右下に `problem_title_ja` の日本語タイトルを表示するカード形式にします。タグは表示しません。AtCoder上のEDPCの実際のcontest IDが `dp` の場合でも、本文管理上は `contest: EDPC` を使ってよいです。


- `problem_title_ja` は任意項目です。問題タイトルの日本語訳を表示したい場合に使います。
  - 通常コンテストの表では、各問題セルの左上に `ABC461 A`、中央に `problem_title`、右下に `problem_title_ja` を表示します。
  - 教育系コンテストページでも、同じ3点配置のカードとして表示します。
  - `problem_title_ja` が空または未指定の場合、右下の日本語タイトルは表示しません。
  - 日本語タイトルは1行表示です。幅に収まらない場合は表示時に少し縮小し、それでも収まらない場合は省略表示にします。
  - 問題番号と日本語タイトルは、補助情報として薄い青色で表示します。
  - 半角括弧などの記号もそのまま使えます。`:` や `#` などが混ざる場合に備えて、値は引用符で囲むのが安全です。
  - 日本語タイトル部分だけ別のツールチップ文になることを避けるため、hover 表示は問題セル全体のタイトルに統一します。
- `problem_url`、`submission_url`、`alternative_submission_urls` はタイトル直下に自動表示します。
- 本文中に同じリンクがなくても問題にしません。
- `tags` が参照している知識記事が見つからない場合、サイト生成スクリプトの実行結果に警告を表示します。
  - 知識記事の `title`、`aliases`、`absorbs` のいずれかに一致すれば対応済みとみなします。
  - `docs/tags/` のタグページ自体は通常どおり生成します。


### リンクの扱い

- `http://` または `https://` で始まるサイト外リンクは、生成HTMLで `target="_blank" rel="noopener noreferrer"` を付け、別ウィンドウまたは別タブで開きます。
  - 問題ページ、解答例、別解提出、外部リンク集、本文中の外部リンクが対象です。
- サイト内リンクは相対パスで生成し、同じウィンドウで開きます。

### 知識記事

知識記事Markdownは `knowledge/` 配下に置きます。`order` は使わず、並び順は `category_order` と `level_order` を使います。

```yaml
---
title: 二分探索
level: C問題相当
category: データ探索系
summary: 条件を満たす境界を高速に探す手法。
category_order: 9999
level_order: 9999
aliases:
  - binary search
  - lower_bound
  - upper_bound
absorbs:
  - lower_bound関数
  - upper_bound関数
related:
  - 解の二分探索
  - 計算量の見積もり
---
```

- `category: 未分類` または `level: 未分類` の記事は、通常導線・タグ検索から外します。
- ページ自体が生成されることは許容します。
- 新規スタブの `summary` と本文の `## 概要` は、どちらも `今後記述予定。` を初期値にします。
- 知識ページ右側のサイドバーには、front matter の `related` から解決した `関連知識` リンクを表示します。
  - 解決は `title`、`aliases`、`absorbs` のいずれかで行います。
  - 解決できない `related` はサイト生成時に警告します。
- `category: 典型問題集` は、問題パターンとして参照したい知識記事に使います。
- `DAG上のDP` は `トポロジカルソート` を吸収して扱います。
- `ツーポインタ法`、`尺取法`、`sliding window法` は別記事として扱います。
  - `尺取法`: `knowledge/shakutori-method.md`
  - `sliding window法`: `knowledge/sliding-window.md`
- `scripts/build_site.py` は `search.json` を生成しません。タグページと知識記事一覧を主な探索導線とします。

### Markdown表示

- Markdown表に対応しています。
- Markdown画像記法に対応しています。
  - 例: `explanations/abc463/e.md` から同じディレクトリの `e1.png` を使う場合、本文に `![説明](e1.png)` と書きます。
  - 生成時に `docs/explanations/abc463/e1.png` へコピーし、HTML側の参照パスを自動補正します。
  - `knowledge/` 側の記事でも、記事Markdownから見た相対パスで画像を参照できます。
- `markdown` パッケージがない環境でも、基本的なMarkdown表と画像はフォールバック変換で表示できます。
- 数式は MathJax 3 で表示します。
  - インライン: `$...$` または `\(...\)`
  - ブロック: `$$...$$` または `\[...\]`
  - 生成HTML内のMathJax設定では、JavaScript文字列として `\(` / `\)` / `\[` / `\]` が正しく渡るようにバックスラッシュを二重に保持します。

## タグ・スラッグ方針

- タグページのファイル名はASCII slugにします。
- 知識記事の `title` と一致するタグは、知識記事ファイル名の stem を優先してslugにします。
  - 例: `knowledge/beam-search.md` の `title: ビームサーチ` は `docs/tags/beam-search.html`。
- `aliases` だけを理由にタグページは作りません。
- 問題解説側で実際に使われている alias / absorb タグは、対応する知識記事をタグページ上部に表示します。

## コンテスト・知識の表示順

問題カテゴリの順序:

1. ABC
2. ARC
3. AGC
4. AHC
5. 典型90問
6. 教育系コンテスト
7. その他AtCoder
8. Aizu Online Judge
9. Project Euler

知識カテゴリーの順序:

1. 基本
2. 変数とデータ構造
3. データ探索系
4. 高速化系
5. 文字列系
6. グラフ理論系
7. 幾何学系
8. その他数学系
9. その他
10. 典型問題集
11. 未分類

知識難易度の順序:

1. 難易度問わず
2. A問題相当
3. B問題相当
4. C問題相当
5. D問題相当
6. E問題相当
7. F問題相当
8. G問題以上相当
9. AHC用
10. 未分類

## 今後の変更時の注意

- ZIP作成時は、原則として `explanations/`、`knowledge/`、`docs/` を含めません。
- 公開向けトップREADMEは `README.md`、GPT向け作業メモは `README_for_GPT.md` に分けます。
- `README.md` の先頭に `cat > README.md <<'EOF'` などのシェルコマンドを混ぜないでください。
- `README.md` の Markdown コードフェンスは必ず閉じてください。
- Project Euler は100番までを対象にし、101番以降は扱わない方針です。表示名は `Project Euler`、本文注記は `（権利の都合上100番まで）` とします。
- 変更履歴やバージョン別の反映内容は、このファイルや `PACKAGE_NOTE.txt` に書き散らさず、必要な場合は `CHANGELOG.md` に集約します。
