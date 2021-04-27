# アーキテクチャ演習
## 概要
本システムは、TCPを用いてTCP/IPのレイヤードアーキテクチャを模擬したシステムである。本システムは、3層からなるレイヤードアーキテクチャで構成される。第一層はデータ層、第二層はDTCPもしくはDUDP層、第三層はDIP層である。

## 第一層
第一層はデータ層である。

この層は、ヘッダとして下記の3フィールドを持つ。

|フィールド|長さ(Bytes)|意味|
|type|4|上位プロトコルタイプを表す|
|version|4|DIPバージョンを示す|
|ttl|4|time to live|

## 第二層
第二層は、DTCPもしくはDUDP用のデータ層である。

### DTCP
この層は、ヘッダとして下記の3フィールドを持つ。ここで、ペイロードのMD5値が不正なパケットと判断された場合、改ざんされていた場合には即座に処理を終了する。

|フィールド|長さ(Bytes)|意味|
|type|4|上位プロトコルタイプを表す|
|len|4|ペイロードの長さ|
|digest|4|ペイロードのMD5値|

### DUDP
この層は、ヘッダとして下記の2フィールドを持つ。

|フィールド|長さ(Bytes)|意味|
|type|4|上位プロトコルタイプを表す|
|len|4|ペイロードの長さ|

## 第一層
この層はデータを管理する。