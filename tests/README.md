# site_builder 回帰テスト

サイト生成処理の変更で既存出力を壊していないことを確認するためのテストです。

通常の確認は、リポジトリ直下で次を実行します。

```text
python -S -m unittest discover -s tests -v
```

`-S` を付け、Python 3.11 以上の標準ライブラリだけを使う経路を基準にしています。
`requirements.txt` に必須の外部依存はありません。

`tests/expected_site_sha256.json` は、小さな固定 fixture から生成される全ファイルの SHA-256 一覧です。
意図的に生成結果を変更した場合だけ、変更内容を確認したうえで次を実行して更新します。

```text
UPDATE_SITE_SNAPSHOT=1 python -S -m unittest discover -s tests -v
```

GitHub Actions では fixture の回帰テストに加え、実リポジトリ全体を再生成し、追跡中の `docs/` と差分がないことも確認します。
