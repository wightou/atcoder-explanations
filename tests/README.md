# site_builder 回帰テスト

サイト生成処理の変更で既存出力を壊していないことを確認するためのテストです。

通常の確認は、リポジトリ直下で次を実行します。

```text
python -m pip install -r requirements.txt
python -m unittest discover -s tests -v
```

Python 3.13 以上を前提とします。
本番サイト生成では `requirements.txt` の PyYAML と Python-Markdown を使用し、fixture の個別テストでは外部パッケージを使わないフォールバック経路も明示的に確認します。

`tests/expected_site_sha256.json` は、小さな固定 fixture から生成される全ファイルの SHA-256 一覧です。
意図的に生成結果を変更した場合だけ、変更内容を確認したうえで次を実行して更新します。

```text
UPDATE_SITE_SNAPSHOT=1 python -m unittest discover -s tests -v
```

GitHub Actions では fixture の回帰テストに加え、実リポジトリ全体を再生成し、追跡中の `docs/` と差分がないことも確認します。
