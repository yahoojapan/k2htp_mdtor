---
layout: contents
language: en-us
title: Usage
short_desc: K2Hash Transaction Plugin for Multiple Distributed Transaction Of Repeater
lang_opp_file: usage.html
lang_opp_word: To English
prev_url: detailsja.html
prev_string: Details
top_url: indexja.html
top_string: TOP
next_url: buildja.html
next_string: Build
---

# 使用法

この章ではGitHubリポジトリにある**k2htp_mdtor**（以下**K2HTPMDTOR**と呼ぶ）の例を使用する方法を説明します。サンプルコードを実行するには2つのステップで十分です。

1. **K2HTPMDTOR**をインストールする
2. サンプルコードを実行する

## **K2HTPMDTOR**をインストールする

このセクションではバイナリパッケージのインストール方法を説明します。ソースコードからバイナリをビルドしたい場合は[Build](https://k2htpmdtor.antpick.ax/buildja.html)ページを読んでください。

DebianStretchまたはUbuntu（Bionic Beaver）をお使いの場合は、以下の手順に従ってください。
```bash
$ sudo apt-get update -y
$ sudo apt-get install curl -y
$ curl -s https://packagecloud.io/install/repositories/antpickax/stable/script.deb.sh \
    | sudo bash
$ sudo apt-get install k2htpmdtor
```

Fedora28またはCentOS7.x（6.x）ユーザーの場合は、以下の手順に従ってください。
```bash
$ sudo yum makecache
$ sudo yum install curl -y
$ curl -s https://packagecloud.io/install/repositories/antpickax/stable/script.rpm.sh \
    | sudo bash
$ sudo yum install k2htpmdtor -y
```

## サンプルコードを実行する

あなたは単に2つのステップが必要です。リポジトリを複製します。そしてサンプルコードを実行します。
```bash
$ git clone https://github.com/yahoojapan/k2htp_mdtor.git
$ cd k2htp_mdtor/tests
$ sh test.sh
                                                                                                                                          
====== Start INI file test =================================                                                                              
------ Initialize test environment for INI file ------------   

....

====== Finish all ==========================================

RESULT --> SUCCEED
```

次の図は、test.shからlibk2htpmdtordmy2.soへの単純な呼び出しの流れを示しています。

1. test.shはk2htpmdtortestをフォークします。
2. k2htpmdtortestのmain関数は、[K2HASH](https://k2hash.antpick.ax/indexja.html)を共有ライブラリとしてロードします。
3. [K2HASH](https://k2hash.antpick.ax/indexja.html)は**K2HTPMDTOR**を共有ライブラリとしてロードし、test.iniを**K2HTPMDTOR**に渡します。
4. **K2HTPMDTOR**はtest.iniを読み込み、トランザクションプラグインをロードします。

```
test.sh
   |
   v
 (fork) --> k2htpmdtortest
               |
               v
            (load) --> k2hash(libk2hash.so)  
                          |
                          |--> (pass) test.ini
                          |              |
                          v              v
                       (load) --> k2htp_mdtor(libk2htpmdtor.so)
                                     |
                                     v
                                   (load) --> transaction plugin(libk2htpmdtordmy.so)
                                     |
                                     v
                                   (load) --> transaction plugin(libk2htpmdtordmy2.so)
...
```

上記のソースと設定はGitHubで入手できます。
* [k2htpmdtortest.cc](https://github.com/yahoojapan/k2htp_mdtor/blob/master/tests/k2htpmdtortest.cc)
* [test.ini](https://github.com/yahoojapan/k2htp_mdtor/blob/master/tests/test.ini)
* [k2htpmdtordmy.cc](https://github.com/yahoojapan/k2htp_mdtor/blob/master/tests/k2htpmdtordmy.cc)
