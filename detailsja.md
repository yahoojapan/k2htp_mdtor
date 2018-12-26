---
layout: contents
language: en-us
title: Details
short_desc: K2Hash Transaction Plugin for Multiple Distributed Transaction Of Repeater
lang_opp_file: details.html
lang_opp_word: To English
prev_url: homeja.html
prev_string: Overview
top_url: indexja.html
top_string: TOP
next_url: usageja.html
next_string: Usage
---

# Details

この章は2つの部分で構成されています。 ひとつは**k2htp_mdtor**（別名**mdtor**）を有効にする方法です。 もう一つは**mdtor**の設定ファイルの詳細です。

## **mdtor**を有効にする方法

有効にするには、2つのステップが必要です。

1. load関数を呼び出してメモリにロードしてください。
2. enable関数を呼び出してトランザクションを有効にします。

### ロードmdtor

**mdtor**は[K2HASH](https://k2hash.antpick.ax)トランザクションログを扱うための共有ライブラリです。アプリケーションは次のAPIを呼び出して**mdtor**をロードします。 APIは現在CおよびC ++で利用可能です。詳細はそれぞれのAPIリファレンスを読んでください。

- C++ API  
[K2HTransDynLib::get](https://k2hash.antpick.ax/developer.html#K2HTRANSDYNLIB)

- C API  
[k2h_load_transaction_library](https://k2hash.antpick.ax/developer.html#DSO)

### トランザクションを有効にする

**mdtor**を正常にロードしたら、次のステップは次のAPIを呼び出してトランザクションログを有効にするように[K2HASH](https://k2hash.antpick.ax)に指示することです。あなたは[K2HASH](https://k2hash.antpick.ax) APIがAPIドキュメントの**mdtor**設定ファイルパスを必要とするのを見るでしょう。心配しないでください。次のセクションで設定ファイルについて説明します。このセクションでは、APIがパスを必要とすることだけを認識してください。

- C++ API  
[K2HShm::EnableTransaction](https://k2hash.antpick.ax/developer.html#-k2hshm-class)

- C API  
[k2h_enable_transaction_param](https://k2hash.antpick.ax/developer.html#-transaction-archive-familyc-i--f)

## **mdtor**の設定ファイル

このセクションでは、設定ファイルの形式とそのパラメータについて説明します。設定ファイルは**mdtor**の動作を決定します。 **mdtor**がトランザクションプラグインを制御する方法

### 設定ファイルの形式

INI、JSON、YAMLはサポートされているファイル形式です。以下の例を参照してください。次のセクションで設定ファイルのパラメータについて説明します。

- [test.ini](https://github.com/yahoojapan/k2htp_mdtor/blob/master/tests/test.ini)
- [test.json](https://github.com/yahoojapan/k2htp_mdtor/blob/master/tests/test.json)
- [test.yaml](https://github.com/yahoojapan/k2htp_mdtor/blob/master/tests/test.yaml)

あなた自身の設定ファイルを作ると仮定します。前のセクションで見たように、C ++でアプリケーションを実装する場合は、ファイルパスを[K2HShm::EnableTransaction](https://k2hash.antpick.ax/developer.html#-k2hshm-class)の最初のパラメータに渡します。アプリケーションをCで実装する場合は、ファイルパスを[k2h_enable_transaction_param](https://k2hash.antpick.ax/developer.html#-transaction-archive-familyc-i--f)の2番目のパラメータに渡します。

### 設定ファイルのパラメータ

このセクションでは、**mdtor**の設定ファイル内のパラメータについて説明します。まだ見たことがない場合は、上記の例が表示されることがあります。

**mdtor**設定ファイルは2つのセクションから構成されています。

1. K2HTPMDTOR  
   このセクションでは、トランザクションプラグインを呼び出す方法を**mdtor**に伝えます。詳細は下記のK2HTPMDTORセクションを参照してください。
2. K2HTPMDTOR_TP  
   このセクションでは、各トランザクションプラグインのパスを**mdtor**に伝えます。詳細は下記のK2HTPMDTOR_TPセクションを参照してください。

#### K2HTPMDTOR  

このセクションのディレクティブは、**mdtor**がトランザクションプラグインを呼び出す方法と、トランザクションプラグインからの結果を処理する方法を決定します。

|section|directive|value|optional|
|:--|:--|:--|:--|
|K2HTPMDTOR|RUN_MODE|ALLはすべてのトランザクションプラグインを順番に呼び出します。 <br /> RANDOMはランダムに選択されたトランザクションプラグインメソッドを呼び出します。 <br /> UNTIL_SUCCESSはトランザクションプラグインメソッドの呼び出しを停止します。メソッドが成功を返すと。 <br /> メソッドが失敗を返した場合、UNTIL_FAILUREはトランザクションプラグインメソッドの呼び出しを停止します。 |いいえ|

#### K2HTPMDTOR_TP  
このセクションのディレクティブは、トランザクションプラグインとその設定ファイルが存在する場所を決定します。

|section|directive|value|optional|
|:--|:--|:--|:--|
|K2HTPMDTOR_TP|TPPATH|トランザクションプラグインへのパス。 libk2htpdtor.soがデフォルト値です。|はい|
|K2HTPMDTOR_TP|TPCONF|トランザクションプラグインへの設定パス。|いいえ|
