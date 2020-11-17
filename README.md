k2htp_mdtor
----------
[![C/C++ AntPickax CI](https://github.com/yahoojapan/k2htp_mdtor/workflows/C/C++%20AntPickax%20CI/badge.svg)](https://github.com/yahoojapan/k2htp_mdtor/actions)
[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://raw.githubusercontent.com/yahoojapan/k2htp_mdtor/master/COPYING)
[![GitHub forks](https://img.shields.io/github/forks/yahoojapan/k2htp_mdtor.svg)](https://github.com/yahoojapan/k2htp_mdtor/network)
[![GitHub stars](https://img.shields.io/github/stars/yahoojapan/k2htp_mdtor.svg)](https://github.com/yahoojapan/k2htp_mdtor/stargazers)
[![GitHub issues](https://img.shields.io/github/issues/yahoojapan/k2htp_mdtor.svg)](https://github.com/yahoojapan/k2htp_mdtor/issues)
[![debian packages](https://img.shields.io/badge/deb-packagecloud.io-844fec.svg)](https://packagecloud.io/antpickax/stable)
[![RPM packages](https://img.shields.io/badge/rpm-packagecloud.io-844fec.svg)](https://packagecloud.io/antpickax/stable)


**K2HTPMDTOR** - **K2H**ash **T**ransaction **P**lugin for **M**ultiple **D**istributed **T**ransaction **O**f **R**epeater

### Overview

**K2HTPMDTOR(k2htp_mdtor)** is a [K2HASH](https://k2hash.antpick.ax) transaction plugin that handles [K2HASH](https://k2hash.antpick.ax) transaction logs. Once an application updates a [K2HASH](https://k2hash.antpick.ax) file, [K2HASH](https://k2hash.antpick.ax) generates a transaction log. **K2HTPMDTOR** receives it and passes it to other [K2HASH](https://k2hash.antpick.ax) transaction plugins.

The main purpose of **K2HTPMDTOR**, which loads multiple [K2HASH](https://k2hash.antpick.ax) transaction plugins, is to call the arbitrary function defined in them. When calling the each function, **K2HTPMDTOR** passes [K2HASH](https://k2hash.antpick.ax) transaction logs to it as a function's parameter. A common [K2HASH](https://k2hash.antpick.ax) transaction plugin loads no other one. We recommend you to use **K2HTPMDTOR** if you want to use multiple [K2HASH](https://k2hash.antpick.ax) transaction plugins. 

**K2HTPMDTOR** is designed to corporate with subsystems. **K2HTPMDTOR** can pass a [K2HASH](https://k2hash.antpick.ax) transaction log to other transaction plugins simultaneously. This can help subsystems to track updates from an application very quickly.  

![K2HTPMDTOR](https://k2htpmdtor.antpick.ax/images/top_k2htpmdtor.png)

Read the following documents about **K2HTPMDTOR** for details.

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

