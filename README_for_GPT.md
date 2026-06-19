# AtCoder Explanations Site - meta only patch v32

このパッケージは、サイト生成スクリプトと設定・仕様書だけを含む差分配布用ZIPです。

## 含まないもの

上書き事故防止のため、次は含めていません。

```txt
explanations/
knowledge/
docs/
```

## 変更点

- `knowledge_item_sort_key` を削除しました。
- タグページのファイル名生成を修正しました。
  - 知識記事の `title` と一致するタグは、知識記事ファイル名の stem を最優先します。
  - 例: `ビームサーチ` は `docs/tags/beam-search.html` になります。
  - `aliases` だけを理由にタグページを作らないようにしました。
  - 問題解説側で実際に使われている alias / absorb タグは、引き続き対応する知識記事をタグページ上部に表示します。
- 旧仕様の `order` は使いません。
  - `category_order`
  - `level_order`
  のみを使います。



- Markdown中の数式表示に対応しました。
  - インライン数式: `$...$` または `\(...\)`
  - ブロック数式: `$$...$$` または `\[...\]`
  - MathJax 3 を読み込んで表示します。
- 右サイドバーやトップページの知識集小項目リンクを、一覧ページ内アンカーではなく個別ページに変更しました。
  - 分野別: `docs/knowledge/categories/*.html`
  - 難易度別: `docs/knowledge/levels/*.html`
  - `by-category.html` / `by-level.html` は全体一覧として残します。

- 知識スタブ生成対象をタグ整理後の名称に合わせました。
  - `ソート` をA問題相当に移動し、`sort関数` と `自力ソート` を吸収します。
  - `絶対値`、`多次元vector`、`多重ループ`、`グラフ理論の基礎`、`木`、`三角比`、`Nim` を反映しました。
  - `解の二分探索` は `二分探索` に吸収します。
  - `拡張ユークリッドの互除法` をD問題相当に変更しました。

## 反映方法

```bash
python scripts/build_site.py
```

知識スタブを作り直す場合は、必要に応じて次を実行します。

```bash
python scripts/create_knowledge_stubs.py
```


## v33 update

- Markdown表の表示を強化しました。
- `markdown` パッケージがない環境でも、基本的なMarkdown表をフォールバック変換で表示できます。
- 表用CSSを追加しました。
