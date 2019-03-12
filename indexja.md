---
layout: default
language: en-us
title: k2htp_mdtor
short_desc: K2Hash Transaction Plugin for Multiple Distributed Transaction Of Repeater
lang_opp_file: index.html
lang_opp_word: To English
arrow_link_overview: <a href="homeja.html" class="link-title"><span class="arrow-base link-arrow-right"></span>Overview</a>
arrow_link_details: <a href="detailsja.html" class="link-title"><span class="arrow-base link-arrow-right"></span>Details</a>
arrow_link_usage: <a href="usageja.html" class="link-title"><span class="arrow-base link-arrow-right"></span>Usage</a>
arrow_link_build: <a href="buildja.html" class="link-title"><span class="arrow-base link-arrow-right"></span>Build</a>
---

# K2HTPMDTOR

**k2htp_mdtor**（以下**K2HTPMDTOR**と呼ぶ）は、[K2HASH](https://k2hash.antpick.ax/indexja.html)トランザクションログを処理する[K2HASH](https://k2hash.antpick.ax/indexja.html)トランザクションプラグインです。 アプリケーションが[K2HASH](https://k2hash.antpick.ax/indexja.html)ファイルを更新すると、[K2HASH](https://k2hash.antpick.ax/indexja.html)はトランザクションログを生成します。 **K2HTPMDTOR**はトランザクションログを受け取り、トランザクションログを他の[K2HASH](https://k2hash.antpick.ax/indexja.html)トランザクションプラグインに渡します。

**K2HTPMDTOR**の目的は、複数の[K2HASH](https://k2hash.antpick.ax/indexja.html)トランザクションプラグインをロードし、1つのトランザクションに対して複数の[K2HASH](https://k2hash.antpick.ax/indexja.html)トランザクションプラグインに定義された特定の関数を同時に実行することです。[K2HASH](https://k2hash.antpick.ax/indexja.html)に登録できる[K2HASH](https://k2hash.antpick.ax/indexja.html)トランザクションプラグインは、1つのみロードでき、プラグインに定義された特定の関数を実行できます。複数の[K2HASH](https://k2hash.antpick.ax/indexja.html)トランザクションプラグインを使用したい場合は、**K2HTPMDTOR**を使用してください。

**K2HTPMDTOR**はサブシステムと連携するように設計されています。 **K2HTPMDTOR**は[K2HASH](https://k2hash.antpick.ax/indexja.html)トランザクションログを他のトランザクションプラグインに同時に渡すことができますので、サブシステムがアプリケーションからの更新を非常に迅速に追跡するのに役立ちます。  

![K2HTPMDTOR](images/top_k2htpmdtor.png)

詳細は**K2HTPMDTOR**に関する以下の文書を読んでください。

## {{ page.arrow_link_overview }}

[概要](homeja.html) ページでは、**K2HTPMDTOR**の概要について説明しています。

## {{ page.arrow_link_details }}

[詳細](detailsja.html) ページでは、**K2HTPMDTOR**の初期化プロセスと設定について説明しています。

## {{ page.arrow_link_usage }}

[使い方](usageja.html) ページには、**K2HTPMDTOR**の使い方の例が記載されています。

## {{ page.arrow_link_build }}

[ビルド](buildja.html) ページは**K2HTPMDTOR**の[ソースコード](https://github.com/yahoojapan/k2htp_mdtor)をビルド方法を指示します。

# **AntPickax** について

[AntPickax](https://antpick.ax/) は [Yahoo Japan Corporation](https://about.yahoo.co.jp/info/en/company/) によるオープンソースプロジェクトです。
