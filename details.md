---
layout: contents
language: en-us
title: Details
short_desc: K2Hash Transaction Plugin for Multiple Distributed Transaction Of Repeater
lang_opp_file: detailsja.html
lang_opp_word: To Japanese
prev_url: home.html
prev_string: Overview
top_url: index.html
top_string: TOP
next_url: usage.html
next_string: Usage
---

# Details

This chapter consists of two parts; one is how to enable **k2htp_mdtor(aka mdtor)**; another is details on **mdtor**'s configuration file.

## How to enable mdtor

You are simply required two steps to enable it.

1. Load it in memory by calling the load function.
2. Enable transaction by calling the enable function.

### Load mdtor

**mdtor** is a shared library to handle [K2HASH](https://k2hash.antpick.ax) transaction logs. An application loads **mdtor** by calling the following API. The APIs are currently available in C and C++. Read the each API reference for details.

- C++ API  
[K2HTransDynLib::get](https://k2hash.antpick.ax/developer.html#K2HTRANSDYNLIB)

- C API  
[k2h_load_transaction_library](https://k2hash.antpick.ax/developer.html#DSO)

### Enable transaction

Once you have successfully loaded **mdtor**, the next step is to tell [K2HASH](https://k2hash.antpick.ax) to enable transaction logging by calling the following API. You will see the [K2HASH](https://k2hash.antpick.ax) API requires a **mdtor** configuration file path in the API documents. Don't worry about it. I describe the configuration file in the next section. You should only recognize the API requires the path in this section.

- C++ API  
[K2HShm::EnableTransaction](https://k2hash.antpick.ax/developer.html#-k2hshm-class)

- C API  
[k2h_enable_transaction_param](https://k2hash.antpick.ax/developer.html#-transaction-archive-familyc-i--f)

## Configuration file of mdtor

This section describes configuration file formats and its parameters. The configuration file determines the **mdtor**'s behavior; how **mdtor** controls transaction plugins.

### Configuration file formats

INI, JSON and YAML are supported file formats. See the following examples. I will describe parameters in them at the next section.

- [test.ini](https://github.com/yahoojapan/k2htp_mdtor/blob/master/tests/test.ini)
- [test.json](https://github.com/yahoojapan/k2htp_mdtor/blob/master/tests/test.json)
- [test.yaml](https://github.com/yahoojapan/k2htp_mdtor/blob/master/tests/test.yaml)

Assume making your own configuration file. As you see in the previous section, you will pass the file path into the [K2HShm::EnableTransaction](https://k2hash.antpick.ax/developer.html#-k2hshm-class)'s first parameter if you implement your application in C++. If you implement your application in C, you will pass it to the [k2h_enable_transaction_param](https://k2hash.antpick.ax/developer.html#-transaction-archive-familyc-i--f)'s second parameter.

### Configuration file parameters

This section describes parameters in **mdtor**'s configuration file. You might see examples above if you haven't seen any of them yet.

A **mdtor** configuration file consists of two sections:

1. K2HTPMDTOR  
   In this section, you tell **mdtor** the way to call transaction plugins. See **K2HTPMDTOR** section below for details.
2. K2HTPMDTOR_TP  
   In this section, you tell **mdtor** each transaction plugin's path. See **K2HTPMDTOR_TP** section below for details.

#### K2HTPMDTOR  
A directive in this section determines how **mdtor** invokes transaction plugins and how it processes results from them.

|section|directive|value|optional|
|:--|:--|:--|:--|
|K2HTPMDTOR|RUN_MODE|**ALL** invokes all transaction plugins in sequence. <br /> **RANDOM** invokes a transaction plugin method randomly selected. <br /> **UNTIL_SUCCESS** stops invoking transaction plugin methods if a method returns a success . <br />**UNTIL_FAILURE** stops invoking transaction plugin methods if a method returns a failure. |No|

#### K2HTPMDTOR_TP  
Directives in this section determine where transaction plugins and their configuration files exist.

|section|directive|value|optional|
|:--|:--|:--|:--|
|K2HTPMDTOR_TP|TPPATH|The path to a transaction plugin. libk2htpdtor.so is the default value.|Yes|
|K2HTPMDTOR_TP|TPCONF|The configuration path to a transaction plugin.|No|
