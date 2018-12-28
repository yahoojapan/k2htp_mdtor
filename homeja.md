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

この章では**k2htp_mdtor**（以下**K2HTPMDTOR**と呼ぶ）の概要について説明します。

**K2HTPMDTOR**は共有ライブラリです。 アプリケーションは[K2HASH](https://k2hash.antpick.ax/indexja.html)ライブラリ関数を呼び出して**K2HTPMDTOR**をロードします。 **K2HTPMDTOR**は複数の[K2HASH](https://k2hash.antpick.ax/indexja.html)トランザクションプラグインをロードして複数の[K2HASH](https://k2hash.antpick.ax/indexja.html)トランザクションプラグインにトランザクションログを渡します。 詳細は[詳細](https://k2htpmdtor.antpick.ax/detailsja.html)ページを読んでください。

次の図は、アプリケーションが[K2HASH](https://k2hash.antpick.ax/indexja.html)ファイルを更新するときのデータフローを示しています。
1. アプリケーションが[K2HASH](https://k2hash.antpick.ax/indexja.html)ファイルのデータの一部を更新します。
2. [K2HASH](https://k2hash.antpick.ax/indexja.html)はトランザクションログを生成し、そのログを使って**K2HTPMDTOR**（別名**K2HTPMDTOR**）のメソッドを呼び出します。
3. **K2HTPMDTOR**はログをトランザクションプラグインに渡します。

![Overview](images/k2htpmdtor_overview.png)

[K2HASH](https://k2hash.antpick.ax/indexja.html)トランザクションプラグインは設定により、トランザクションログを同時にまたは順番に処理します。前のプラグインでシーケンス内のエラーが発生した場合、**K2HTPMDTOR**は設定により、[K2HASH](https://k2hash.antpick.ax/indexja.html)トランザクションプラグインの処理を停止できます。設定に関する[詳細](https://k2htpmdtor.antpick.ax/detailsja.html)ページを読んでください。
