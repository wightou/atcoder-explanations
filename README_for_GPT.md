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
  - 各問題ページでは、英語タイトルの直下に日本語タイトルを表示します。PCなど横幅の広い画面では右揃え、横幅が狭い画面では左揃えにします。
  - 通常コンテストの表では、各問題セルの左上に `ABC461 A`、中央に `problem_title`、右下に `problem_title_ja` を表示します。
  - 教育系コンテストページでも、同じ3点配置のカードとして表示します。
  - `problem_title_ja` が空または未指定の場合、日本語タイトルは表示しません。
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
- 問題解説・知識記事本文の `## 関連知識` 節では、`###` 見出しが既存知識記事の `title` と完全一致する場合、その見出しを該当記事へのリンクにします。
- 問題解説・知識記事本文で、`「XXX」の記事参照。` または `「XXX」「YYY」...の記事参照。` の形が現れ、各記事名が既存知識記事の `title` と一致する場合、それぞれの記事名だけを該当記事へのリンクにします。
  - 鍵括弧より前の文言は問いません。例えば `基本的なナップサック問題については、「ナップサック問題」の記事参照。` でもリンクします。
  - 複数記事を書く場合も `「XXX」「YYY」` のように鍵括弧を連続させます。
  - 1つでも解決できない記事名がある場合は部分リンクせず、その参照表現をそのまま表示します。
  - aliases、absorbs は本文自動リンクの対象にしません。
  - 既存リンクやコード内には自動リンクを作りません。
- `category: 典型問題集` は、問題パターンとして参照したい知識記事に使います。DPの派生手法そのものは入れません。
- DP系の記事は `高速化系` に分類します。
  - `動的計画法`、`木DP`、`bitDP`、`区間DP`、`DAG上のDP`: `D問題相当`。
  - `桁DP`、`全方位木DP`: `E問題相当`。
- `DAG上のDP` は `トポロジカルソート` を吸収して扱います。
- `分枝限定法` は `G問題以上相当` / `高速化系` の独立記事として扱います。slug は `branch-and-bound` です。
- `期待値` は `D問題相当` / `その他数学系` の独立記事として扱います。slug は `expected-value` です。
  - `期待値の線形性` は `期待値` の `absorbs` に入れ、`確率` からは外します。
- `対戦ゲーム` は `D問題相当` / `その他数学系` の独立記事として扱います。slug は `competitive-game` です。
  - `二人零和有限確定完全情報ゲーム` は `aliases` に入れます。
  - `Nim` とは別記事として扱います。
- `auto型` は `B問題相当` / `変数とデータ構造` の独立記事として扱います。slug は `auto` です。`auto` は `aliases` に入れます。
- `size_t型` は `B問題相当` / `変数とデータ構造` の独立記事として扱います。slug は `size-t` です。`size_t` と `std::size_t` は `aliases` に入れます。
- `イテレータ` は `B問題相当` / `変数とデータ構造` の独立記事として扱います。slug は `iterator` です。`iterator` と `イテレーター` は `aliases` に入れます。
- `カウンティング` は `B問題相当` / `基本` の独立記事として扱います。slug は `counting-technique` です。
- `挿入DP` は `F問題相当` / `高速化系` の独立記事として扱います。slug は `insertion-dp` です。
- `凸包` は `F問題相当` / `幾何学系` の独立記事として扱います。slug は `convex-hull` です。
- `最大フロー` は `G問題以上相当` / `グラフ理論系` の独立記事として扱います。`最小カット` は独立記事を作らず、`最大フロー` の `absorbs` で吸収します。
- `プロジェクト選択問題` は `G問題以上相当` / `典型問題集` の独立記事として扱います。slug は `project-selection-problem` です。`燃やす埋める問題` は `aliases` に入れ、関連知識には `最大フロー` を入れます。
- `最大閉包問題` は `G問題以上相当` / `典型問題集` の独立記事として扱います。slug は `maximum-closure-problem` です。関連知識には `プロジェクト選択問題` と `最大フロー` を入れます。
- `形式的冪級数` は `G問題以上相当` / `その他数学系` の独立記事として扱います。既存 URL との互換性のため slug は `fps-convolution` のまま維持し、`畳み込み`、`convolution`、`NTT` は `absorbs` で吸収します。`FPS` は `aliases` に入れます。
- `イベントソート` は独立記事を作らず、`シミュレーション` の `absorbs` で吸収します。
- `バックトレース` は `D問題相当` / `データ探索系` の独立記事として扱います。
  - `経路復元`、`解の復元` は `バックトレース` の別名です。
  - 深さ優先探索に吸収する `バックトラック` とは別概念です。
- `ツーポインタ法`、`尺取法`、`sliding window法` は別記事として扱います。
  - `尺取法`: `knowledge/shakutori-method.md`
  - `sliding window法`: `knowledge/sliding-window.md`
- `scripts/build_site.py` は `search.json` を生成しません。タグページと知識記事一覧を主な探索導線とします。

### Markdown表示

- Markdown表に対応しています。
  - 各表の直前には `<!-- table-row-header: true -->` または `<!-- table-row-header: false -->` を必ず書き、左列も行見出しかどうかを明示します。
  - `true` の場合、本文行の左端セルを `<th scope="row">` として出力し、見出し用の装飾を適用します。`false` の場合は通常の `<td>` のままです。
  - 指定がない表、`true` / `false` 以外の指定、表に結び付かない指定は警告します。指定漏れは後方互換のため `false` として処理します。
  - 表の横幅は本文幅いっぱいに強制せず、セル内容に応じた幅で左寄せ表示します。
  - 表が本文領域より広い場合は、表の範囲内で左右スクロールできます。
  - 区切り行のコロンで列の配置を指定できます。見出しセルと本文セルの両方へ適用されます。
    - `|:---|`: 左寄せ
    - `|:---:|`: 中央寄せ
    - `|---:|`: 右寄せ
- 表セルの先頭に色指定を書くと、そのセルの背景色を変更できます。
  - 例:

    ```md
    | 値1 | 値2 |
    |---|---|
    | {red}$5$ | {blue}`3` |
    ```
  - 色指定はセルの先頭に置き、`{色名}` の直後に通常どおりセル内容を書きます。
  - 見出しセルと通常セルの両方で使用できます。
  - 背景色は CSS class とインライン `style` の両方へ出力します。文字色は通常の本文色のままです。
  - 対応色: `red`, `coral`, `orange`, `amber`, `yellow`, `olive`, `lime`, `green`, `emerald`, `teal`, `cyan`, `blue`, `navy`, `indigo`, `violet`, `purple`, `magenta`, `rose`, `pink`, `brown`, `gray`, `black`
  - 未対応の色名は変換されず、`{色名}` がそのまま表示されます。
  - 色だけに説明を依存させず、必要なら本文で色の意味も説明します。
- Markdown の箇条書きは複数階層に対応しています。
  - `- parent` の下に `  - child` のように2スペース以上インデントすると子階層として扱います。
  - 表示記号は1階層目が黒丸、2階層目が白丸、3階層目以降が四角です。
- 問題解説・知識記事では、本文中の `##` から `######` までの見出しからページ上部に目次を自動生成します。
  - 見出しの階層を保って表示し、各項目から本文の該当位置へリンクします。
  - `関連知識` 節は、その節自身だけを目次に載せ、配下の下位見出しは目次に載せません。
- Markdown画像記法に対応しています。
  - 例: `explanations/abc463/e.md` から同じディレクトリの `e1.png` を使う場合、本文に `![説明](e1.png)` と書きます。
  - 生成時に `docs/explanations/abc463/e1.png` へコピーし、HTML側の参照パスを自動補正します。
- `explanations/` 配下には、解説 Markdown の `.md`、保存する解答コードの `.cpp`、解説画像の `.png` を置けます。
- それ以外のファイルがある場合、`build_site.py` は実行ファイルなどの混入を疑って警告します。警告後もビルドは継続します。
- `explanations/` / `knowledge/` の Markdown で、front matter 欠落、YAML不正、必須項目不足、`tags` / `aliases` / `absorbs` / `related` の型不正、`category_order` / `level_order` の型不正など、その1ファイルに閉じた形式不正が見つかった場合は `WARNING` を出してそのファイルだけスキップし、残りの生成を継続します。
  - 想定外の内部例外や、出力ディレクトリ操作などサイト全体の生成処理の失敗までは握りつぶしません。
  - `knowledge/` 側の記事でも、記事Markdownから見た相対パスで画像を参照できます。
- `markdown` パッケージがない環境でも、基本的なMarkdown表と画像はフォールバック変換で表示できます。
- 数式は MathJax 3 で表示します。
  - インライン: `$...$` または `\(...\)`
  - ブロック: `$$...$$` または `\[...\]`
  - 生成HTML内のMathJax設定では、JavaScript文字列として `\(` / `\)` / `\[` / `\]` が正しく渡るようにバックスラッシュを二重に保持します。
  - 知識記事本文だけでなく、front matter の `summary` に数式がある場合も、知識一覧・分類別一覧・タグページでMathJaxを読み込みます。

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
