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

# k2htp_mdtor

**k2htp_mdtor**（別名**mdtor**）は、[K2HASH](https://k2hash.antpick.ax)トランザクションログを処理する[K2HASH](https://k2hash.antpick.ax)トランザクションプラグインです。 アプリケーションが[K2HASH](https://k2hash.antpick.ax)ファイルを更新すると、[K2HASH](https://k2hash.antpick.ax)はトランザクションログを生成します。 **mdtor**はトランザクションログを受け取り、トランザクションログを他の[K2HASH](https://k2hash.antpick.ax)トランザクションプラグインに渡します。

**mdtor**の目的は、複数の[K2HASH](https://k2hash.antpick.ax)トランザクションプラグインをロードし、1つのトランザクションに対して複数の[K2HASH](https://k2hash.antpick.ax)トランザクションプラグインに定義された特定の関数を同時に実行することです。[K2HASH](https://k2hash.antpick.ax)に登録できる[K2HASH](https://k2hash.antpick.ax)トランザクションプラグインは、1つのみロードでき、プラグインに定義された特定の関数を実行できます。複数の[K2HASH](https://k2hash.antpick.ax)トランザクションプラグインを使用したい場合は、**mdtor**を使用してください。

**mdtor**はサブシステムと連携するように設計されています。 **mdtor**は[K2HASH](https://k2hash.antpick.ax)トランザクションログを他のトランザクションプラグインに同時に渡すことができますので、サブシステムがアプリケーションからの更新を非常に迅速に追跡するのに役立ちます。

詳細は**mdtor**に関する以下の文書を読んでください。

## {{ page.arrow_link_overview }}

[概要](homeja.html) ページでは、**mdtor**の概要について説明しています。

## {{ page.arrow_link_details }}

[詳細](detailsja.html) ページでは、**mdtor**の初期化プロセスと設定について説明しています。

## {{ page.arrow_link_usage }}

[使い方](usageja.html) ページには、**mdtor**の使い方の例が記載されています。

## {{ page.arrow_link_build }}

[ビルド](buildja.html) ページは**mdtor**のビルド方法を指示します。

# **AntPickax** について

[AntPickax](https://antpick.ax/) は [Yahoo Japan Corporation](https://about.yahoo.co.jp/info/en/company/) によるオープンソースプロジェクトです。
