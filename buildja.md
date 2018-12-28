---
layout: contents
language: en-us
title: Build
short_desc: K2Hash Transaction Plugin for Multiple Distributed Transaction Of Repeater
lang_opp_file: build.html
lang_opp_word: To English
prev_url: usageja.html
prev_string: Usage
top_url: indexja.html
top_string: TOP
next_url: 
next_string: 
---

# ビルド

この章は3つの部分から構成されています。

* ローカル開発用に**k2htp_mdtor**（以下**K2HTPMDTOR**と呼ぶ）を設定する方法
* ソースコードから**K2HTPMDTOR**を構築する方法
* **K2HTPMDTOR**のインストール方法

## 1. 前提条件をインストールする

**K2HTPMDTOR**は主に、FULLOCK](https://fullock.antpick.ax/indexja.html)、[K2HASH](https://k2hash.antpick.ax/indexja.html)、および[CHMPX](https://chmpx.antpick.ax/indexja.html)に依存します。それぞれの依存ライブラリとヘッダファイルは**K2HTPMDTOR**をビルドするために必要です。それぞれの依存ライブラリとヘッダファイルをインストールする方法は2つあります。好きなものを選ぶことができます。

* [GitHub](https://github.com/yahoojapan)を使う  
  依存ライブラリのソースコードとヘッダファイルをインストールします。あなたはそれぞれの依存ライブラリとヘッダファイルをビルドしてインストールします。
* [packagecloud.io](https://packagecloud.io/antpickax/stable/) を使用する  
  依存ライブラリのパッケージとヘッダファイルをインストールします。あなたはそれぞれの依存ライブラリとヘッダファイルをインストールするだけです。ライブラリはすでに構築されています。

### 1.1. GitHubから各依存ライブラリとヘッダファイルをインストールします。

詳細については以下の文書を読んでください。

* [FULLOCK](https://fullock.antpick.ax/buildja.html)
* [K2HASH](https://k2hash.antpick.ax/buildja.html)
* [CHMPX](https://chmpx.antpick.ax/buildja.html)

### 1.2. packagecloud.ioから各依存ライブラリとヘッダファイルをインストールします。

このセクションでは、[packagecloud.io](https://packagecloud.io/antpickax/stable/)から各依存ライブラリとヘッダーファイルをインストールする方法を説明します。

注：前のセクションで各依存ライブラリとGitHubからのヘッダーファイルをインストールした場合は、このセクションを読み飛ばしてください。

DebianStretchまたはUbuntu（Bionic Beaver）をお使いの場合は、以下の手順に従ってください。
```bash
$ sudo apt-get update -y
$ sudo apt-get install curl -y
$ curl -s https://packagecloud.io/install/repositories/antpickax/stable/script.deb.sh \
    | sudo bash
$ sudo apt-get install autoconf autotools-dev gcc g++ make gdb libtool pkg-config \
    libyaml-dev libfullock-dev k2hash-dev chmpx-dev -y
$ sudo apt-get install git -y
```

Fedora28またはCentOS7.x（6.x）ユーザーの場合は、以下の手順に従ってください。
```bash
$ sudo yum makecache
$ sudo yum install curl -y
$ curl -s https://packagecloud.io/install/repositories/antpickax/stable/script.rpm.sh \
    | sudo bash
$ sudo yum install autoconf automake gcc gcc-c++ gdb make libtool pkgconfig \
    libyaml-devel libfullock-devel k2hash-devel chmpx-devel -y
$ sudo yum install git -y
```

## 2. GitHubからソースコードを複製する

GitHubから**K2HTPMDTOR**のソースコードをダウンロードしてください。
```bash
$ git clone https://github.com/yahoojapan/k2htp_mdtor.git
```

## 3. ビルドしてインストールする

以下の手順に従って**K2HTPMDTOR**をビルドしてインストールしてください。 [GNU Automake](https://www.gnu.org/software/automake/)を使って**K2HTPMDTOR**を構築します。
```bash
$ cd k2htp_mdtor
$ sh autogen.sh
$ ./configure --prefix=/usr
$ make
$ sudo make install
```

**K2HTPMDTOR**のインストールが成功すると、**K2HTPMDTOR**のマニュアルページが表示されます。
```bash
$ man k2htpmdtor
```

