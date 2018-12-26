k2htp_mdtor
----------
[![Build Status](https://travis-ci.org/yahoojapan/k2htp_mdtor.svg?branch=master)](https://travis-ci.org/yahoojapan/k2htp_mdtor)
[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://raw.githubusercontent.com/yahoojapan/k2htp_mdtor/master/COPYING)
[![GitHub forks](https://img.shields.io/github/forks/yahoojapan/k2htp_mdtor.svg)](https://github.com/yahoojapan/k2htp_mdtor/network)
[![GitHub stars](https://img.shields.io/github/stars/yahoojapan/k2htp_mdtor.svg)](https://github.com/yahoojapan/k2htp_mdtor/stargazers)
[![GitHub issues](https://img.shields.io/github/issues/yahoojapan/k2htp_mdtor.svg)](https://github.com/yahoojapan/k2htp_mdtor/issues)
[![debian packages](https://img.shields.io/badge/deb-packagecloud.io-844fec.svg)](https://packagecloud.io/antpickax/stable)
[![RPM packages](https://img.shields.io/badge/rpm-packagecloud.io-844fec.svg)](https://packagecloud.io/antpickax/stable)


**k2htp_mdtor** - **K2H**ash **T**ransaction **P**lugin for **M**ultiple **D**istributed **T**ransaction **O**f **R**epeater

### Overview

**k2htp_mdtor(aka mdtor)** is a [K2HASH](https://k2hash.antpick.ax) transaction plugin that handles [K2HASH](https://k2hash.antpick.ax) transaction logs. Once an application updates a [K2HASH](https://k2hash.antpick.ax) file, [K2HASH](https://k2hash.antpick.ax) gerenates a transaction log. **mdtor** receives it and passes it to other [K2HASH](https://k2hash.antpick.ax) transaction plugins.

The main purpose of **mdtor**, which loads multiple [K2HASH](https://k2hash.antpick.ax) transaction plugins, is to call the arbitrary function defined in them. When calling the each function, **mdtor** passes [K2HASH](https://k2hash.antpick.ax) transaction logs to it as a function's parameter. A common [K2HASH](https://k2hash.antpick.ax) transaction plugin loads no other one. We recommend you to use **mdtor** if you want to use multiple [K2HASH](https://k2hash.antpick.ax) transaction plugins. 

**mdtor** is designed to cooprate with subsystems. **mdtor** can pass a [K2HASH](https://k2hash.antpick.ax) transaction log to other transaction plugins simultaneously. This can help subsystems to track updates from an application very quickly.

Read the following documents about **mdtor** for details.

### Documents
  - [Document top page](https://k2htpmdtor.antpick.ax/)
  - [Github wiki page](https://github.com/yahoojapan/k2htp_mdtor/wiki)
  - [About AntPickax](https://antpick.ax/)

### Packages
  - [RPM packages(packagecloud.io)](https://packagecloud.io/antpickax/stable)
  - [Debian packages(packagecloud.io)](https://packagecloud.io/antpickax/stable)

### License
This software is released under the MIT License, see the license file.

### AntPickax
[AntPickax](https://antpick.ax/) is an open source project by [Yahoo Japan Corporation](https://about.yahoo.co.jp/info/en/company/).

Copyright(C) 2016 Yahoo Japan Corporation.

