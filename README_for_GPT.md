# GPT作業メモ - AtCoder自筆解説集

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

知識記事のスタブを作り直す場合だけ、次を実行します。

```bash
python scripts/create_knowledge_stubs.py
```

Windows由来の `*:Zone.Identifier` ファイルを消す場合は、リポジトリ直下で次を実行します。

```bash
./scripts/delete_zone_identifier.sh
```

## 最新反映内容

### v38

- `scripts/delete_zone_identifier.sh` を追加しました。
  - リポジトリ直下から `./scripts/delete_zone_identifier.sh` で実行します。
  - 中身は `find . -name '*:Zone.Identifier' -type f -delete` のみです。
- Project Euler の表示名を `Project Euler` にしました。
  - タイトル・ナビゲーション・一覧カードから `（100番まで）` を外します。
  - `contests/project-euler-100.html` の本文に `（権利の都合上100番まで）` を表示します。
  - 内部分類IDは既存データとの互換のため `Project Euler（100番まで）` のままです。

### v37

- `.gitignore` を公開リポジトリ側の内容に合わせました。
- 公開向け `README.md` を維持しました。
  - `# AtCoder自筆解説集` から始まる内容です。
  - 先頭の不要なシェルコマンド行は含めません。
  - ディレクトリ構成のコードフェンスは閉じています。
- コンテスト一覧表の問題タイトルを少し小さくしました。
  - `.problem-title { font-size: 0.82rem; line-height: 1.2; }`
  - 問題番号は `.problem-code` として小さく表示します。
  - 問題タイトルは1行で省略し、全文は `title` 属性に入れます。
- `README_for_GPT.md` と `PACKAGE_NOTE.txt` を、次回作業で参照しやすい構成に整理しました。

### v35-v36 から継続している内容

- コンテスト一覧表の各問題セルは次の構造で出力します。

```html
<a class="problem-cell" href="..." title="ABC462 C - Not Covered Points">
  <span class="problem-code">ABC462C</span>
  <span class="problem-title">Not Covered Points</span>
</a>
```

- 長い問題タイトルは折り返さず、`text-overflow: ellipsis;` で省略します。
- `README_for_GPT.md` はトップ表示用ではなく、GPT向け作業メモとして残します。

## サイト生成スクリプトの主な仕様

### 問題解説

- 問題解説Markdownは `explanations/` 配下に置きます。
- front matter の基本項目は次の通りです。

```yaml
---
contest: ABC462
problem: A
problem_title: "Secret Numbers"
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

- `problem_url`、`submission_url`、`alternative_submission_urls` はタイトル直下に自動表示します。
- 本文中に同じリンクがなくても問題にしません。

### 知識記事

- 知識記事Markdownは `knowledge/` 配下に置きます。
- `order` は使いません。並び順は `category_order` と `level_order` を使います。
- 基本形は次の通りです。

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

- `category: 未分類` または `level: 未分類` の記事は、通常導線・タグ検索・検索データから外します。
- ページ自体が生成されることは許容します。

### Markdown表示

- Markdown表に対応しています。
- `markdown` パッケージがない環境でも、基本的なMarkdown表はフォールバック変換で表示できます。
- 数式は MathJax 3 で表示します。
  - インライン: `$...$` または `\(...\)`
  - ブロック: `$$...$$` または `\[...\]`

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
6. DPコンテスト
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
10. 未分類

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
