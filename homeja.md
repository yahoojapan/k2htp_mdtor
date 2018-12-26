---
layout: contents
language: en-us
title: Overview
short_desc: K2Hash Transaction Plugin for Multiple Distributed Transaction Of Repeater
lang_opp_file: home.html
lang_opp_word: To English
prev_url: 
prev_string: 
top_url: indexja.html
top_string: TOP
next_url: detailsja.html
next_string: Details
---

# 概要

この章では**k2htp_mdtor**（別名**mdtor**）の概要について説明します。

**mdtor**は共有ライブラリです。 アプリケーションは[K2HASH](https://k2hash.antpick.ax)ライブラリ関数を呼び出して**mdtor**をロードします。 **mdtor**は複数の[K2HASH](https://k2hash.antpick.ax)トランザクションプラグインをロードして複数の[K2HASH](https://k2hash.antpick.ax)トランザクションプラグインにトランザクションログを渡します。 詳細は[詳細](https://k2htpmdtor.antpick.ax/detailsja.html)ページを読んでください。

次の図は、アプリケーションが[K2HASH](https://k2hash.antpick.ax)ファイルを更新するときのデータフローを示しています。
1. アプリケーションが[K2HASH](https://k2hash.antpick.ax)ファイルのデータの一部を更新します。
2. [K2HASH](https://k2hash.antpick.ax)はトランザクションログを生成し、そのログを使って**K2HTPMDTOR**（別名**mdtor**）のメソッドを呼び出します。
3. **mdtor**はログをトランザクションプラグインに渡します。

![Overview](images/k2htpmdtor_overview.png)

[K2HASH](https://k2hash.antpick.ax)トランザクションプラグインは設定により、トランザクションログを同時にまたは順番に処理します。前のプラグインでシーケンス内のエラーが発生した場合、**mdtor**は設定により、[K2HASH](https://k2hash.antpick.ax)トランザクションプラグインの処理を停止できます。設定に関する[詳細](https://k2htpmdtor.antpick.ax/detailsja.html)ページを読んでください。
