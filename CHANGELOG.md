# CHANGELOG

## v48

- 問題表示で、コンテスト名と問題記号の間に半角スペースを入れるように変更。
  - 個別解説ページ上部を `EDPCB` ではなく `EDPC B` の形で表示。
  - ABC などの問題一覧表も `ABC461A` ではなく `ABC461 A` の形で表示。
  - 教育系コンテスト一覧も同じ表記に統一。

## v47

- 教育系コンテストページを、表ではなくコンテスト別の簡潔な一覧表示に変更。
  - 問題記号とタイトルを1つのリンクにまとめる。
  - タグは表示しない。
- `http://` / `https://` のサイト外リンクに `target="_blank" rel="noopener noreferrer"` を付け、別ウィンドウまたは別タブで開くように変更。
  - サイト内リンクは従来どおり同じウィンドウで開く。

## v46

- `EDPC` / `TDPC` / `NDPC` / `FPS24` を `教育系コンテスト` として扱うように変更。
- `contests/educational.html` を生成し、その中で各コンテストごとに表を分けて表示するように変更。
- Markdown配置は `explanations/edpc/`、`explanations/tdpc/`、`explanations/ndpc/`、`explanations/fps24/` のように別ディレクトリを維持する方針に整理。

## v45

- `C++のコードの書き方` を `難易度問わず` / `基本` の知識記事として追加。
  - slug は `cpp-code-style`。
- 知識ページ右側のサイドバーに `related` から解決した `関連知識` リンクを表示するように変更。
  - `title` / `aliases` / `absorbs` のいずれかで関連先を解決する。
  - 解決できない `related` がある場合は、サイト生成時に警告を表示する。

## v44

- `カタラン数` を `G問題以上相当` / `その他数学系` の知識記事として追加。
  - slug は `catalan-number`。
  - `一般化カタラン数` は `カタラン数` で吸収する。

## v43

- `尺取法` と `sliding window法` を別記事として扱うように変更。
  - `尺取法` の slug は `shakutori-method`。
  - `sliding window法` の slug は `sliding-window`。
- 旧 slug `two-pointers` は `shakutori-method` へリネームするように変更。
- 旧 v41 / v42 の `knowledge/sliding-window.md` が `title: 尺取法` の場合、本文を保持して `knowledge/shakutori-method.md` へ条件付きリネームする処理を追加。
- `scripts/build_site.py` で `docs/search.json` を生成しないように変更。
- `scripts/create_knowledge_stubs.py` の skip 出力は既存ファイルの中身を表示せず、既定では件数だけ表示するように変更。

## v42

- `ツーポインタ法` の category を `高速化系` から `データ探索系` に変更。
- `Moアルゴリズム`、`平方分割`、`分割統治法` の category を `高速化系` に変更。
- `イベントソート` と `貪欲法` は `データ探索系` のまま扱う方針を明記。

## v41

- `尺取法` の slug を `two-pointers` から `sliding-window` に変更。
- 既存の `knowledge/two-pointers.md` は `create_knowledge_stubs.py` 実行時に `knowledge/sliding-window.md` へリネーム。


## v40

- `ツーポインタ法` を `C問題相当` / `高速化系` の知識記事として追加。
- `尺取法` から `two pointers` 系の別名を外し、`ツーポインタ法` との関連に整理。
- 知識カテゴリの表示順を調整し、`典型問題集` を `その他` の下に移動。
- 変更履歴を `CHANGELOG.md` に集約する運用に整理。
- 画像掲載対応、`超頂点`、典型問題集、`階乗`、`二項係数` など、直近のメタ更新を含む。
