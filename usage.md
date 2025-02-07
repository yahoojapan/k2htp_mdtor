---
layout: contents
language: en-us
title: Usage
short_desc: K2Hash Transaction Plugin for Multiple Distributed Transaction Of Repeater
lang_opp_file: usageja.html
lang_opp_word: To Japanese
prev_url: details.html
prev_string: Details
top_url: index.html
top_string: TOP
next_url: build.html
next_string: Build
---

# Usage

This chapter instructs how to use a **k2htp_mdtor**(aka **K2HTPMDTOR**) example at the **K2HTPMDTOR** repository on GitHub. Only two steps are enough to run the example code.

1. Install **K2HTPMDTOR**
2. Run the example code

## Install mdtor

This section instructs how to install binary packages. Read the [Build](https://k2htpmdtor.antpick.ax/build.html) page if you want to build the binary from the source code.

### For Debian-based Linux distributions users, follow the steps below:
```
$ sudo apt-get update -y
$ sudo apt-get install curl -y
$ curl -s https://packagecloud.io/install/repositories/antpickax/stable/script.deb.sh | sudo bash
$ sudo apt-get install k2htpmdtor
```

### For RPM-based Linux distributions users, follow the steps below:
```
$ sudo dnf makecache
$ sudo dnf install curl -y
$ curl -s https://packagecloud.io/install/repositories/antpickax/stable/script.rpm.sh | sudo bash
$ sudo dnf install k2htpmdtor -y
```

### For ALPINE-based Linux distributions users, follow the steps below:
```
# apk update
# apk add curl
# curl -s https://packagecloud.io/install/repositories/antpickax/stable/script.alpine.sh | sh
# apk add k2htpmdtor
```

## Run the example code

You are simply required two steps; Clone the repository; and run the example code.

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

The following figure shows the simple call flow from test.sh to libk2htpmdtordmy2.so, which describes that:

1. test.sh forks k2htpmdtortest.
2. The main function in k2htpmdtortest loads [K2HASH](https://k2hash.antpick.ax) as a shared library.
3. [K2HASH](https://k2hash.antpick.ax) loads **K2HTPMDTOR** as a shared library and pass test.ini to **K2HTPMDTOR**.
4. **K2HTPMDTOR** reads test.ini and loads transaction plugins.

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

The sources and the settings above are available at GitHub.com:
* [k2htpmdtortest.cc](https://github.com/yahoojapan/k2htp_mdtor/blob/master/tests/k2htpmdtortest.cc)
* [test.ini](https://github.com/yahoojapan/k2htp_mdtor/blob/master/tests/test.ini)
* [k2htpmdtordmy.cc](https://github.com/yahoojapan/k2htp_mdtor/blob/master/tests/k2htpmdtordmy.cc)
