# Raspberry Pi Pico W用espress_stream

<div style="text-align:right">2026年9月20日</div>

## 概要

[espress_stream](http://akino-yo.f5.si/akinoyonika/espress_stream/)の[Raspberry Pi Pico W](https://www.raspberrypi.com/products/raspberry-pi-pico/)(以降Pico W)上への移植を行いました。
linux用とは以下のような違いがあります。

- シングルタスク、スレッド未使用。
- リソース固定、ヒープ未使用。
- 排他制御、同期機構関連部のダミー実装による無効化もしくは疑似動作化。
- lwIPによるデータグラム通信部。
- SDKのライブラリーによるティック生成部。
- SDKのライブラリーによる乱数生成部。

その他、小リソース向けに設定値を変更しています。

なお、開発環境には[Raspberry Pi Pico C/C++ SDK](https://datasheets.raspberrypi.com/picow/connecting-to-the-internet-with-pico-w.pdf)を用いました。

## パッケージ

本ソフトウェアはソースパッケージにて提供しています。以下をダウンロードしてください。

- [espress_stream_for_pico_w.20260920.tar.gz](http://akino-yo.f5.si/akinoyonika/espress_stream_for_pico_w/espress_stream_for_pico_w.20260920.tar.gz)

<details><summary>変更履歴</summary>

espress_stream_for_pico_w.20260920.tar.gz (1.12-1)
: <ul>
    <li><tt>es_ccat</tt>の入出力をハードウェアシリアルにした時の<tt>read</tt>と<tt>write</tt>の動作を改善。</li>
  </ul>

espress_stream_for_pico_w.20260913.tar.gz (1.12-1)
: <ul>
    <li>CPUの動作クロックを250Mhzにオーバークロックした。</li>
  </ul>

espress_stream_for_pico_w.20260901.tar.gz (1.12-1)
: <ul>
    <li><tt>espress_stream_dgram.c</tt>を改良</li>
  </ul>

espress_stream_for_pico_w.20260825.tar.gz (1.12-1)
: <ul>
    <li><tt>es_ccat</tt>の<tt>read</tt>と<tt>write</tt>を標準ライブラリーの仕様に近づけた。</li>
  </ul>

espress_stream_for_pico_w.20260805.tar.gz (1.12-1)
: <ul>
    <li>ネットワーク関連設定を<tt>net_config.h</tt>に集約。</li>
  </ul>

espress_stream_for_pico_w.20260722.tar.gz (1.12-1)
: <ul>
    <li><tt>es_ccat</tt>の僅かな修正。</li>
  </ul>

espress_stream_for_pico_w.20260701.tar.gz (1.12-1)
: <ul>
    <li><tt>CMakeLists.txt</tt>を整理し、<tt>espress_stream_import.sh</tt>を改良。ソフトウェア本体には変更なし。</li>
  </ul>

espress_stream_for_pico_w.20260311.tar.gz (1.12-1)
: <ul>
    <li>ドキュメントの加筆、修正。</li>
  </ul>

espress_stream_for_pico_w.20251001.tar.gz (1.12-1)
: <ul>
    <li>espress_streamのバージョンアップに対応した。</li>
  </ul>

espress_stream_for_pico_w.20250928.tar.gz (1.11-1)
: <ul>
    <li>ドキュメントの修正。</li>
  </ul>

espress_stream_for_pico_w.20250919.tar.gz (1.11-1)
: <ul>
    <li>espress_streamのバージョンアップに対応した。</li>
    <li>サンプルアプリケーション<tt>es_ccat</tt>を新設。</li>
  </ul>

espress_stream_for_pico_w.20250903.tar.gz (1.10-1)
: <ul>
    <li>espress_streamのバージョンアップに対応した。</li>
    <li><tt>espress_stream_dgram.c</tt>を改良。</li>
  </ul>

espress_stream_for_pico_w.20250805.tar.gz (1.9-1)
: <ul>
    <li>前パッケージでバージョン番号の更新を忘れていたので、更新しパッケージ再作成。</li>
  </ul>

espress_stream_for_pico_w.20250731.tar.gz (1.9-1)
: <ul>
    <li>espress_streamのバージョンアップに対応した。</li>
  </ul>

espress_stream_for_pico_w.20250201.tar.gz (1.8-1)
: <ul>
    <li>espress_streamのバージョンアップに対応した。</li>
  </ul>

espress_stream_for_pico_w.20241224.tar.gz (1.7-1)
: <ul>
    <li>espress_streamのバージョンアップに対応した。</li>
  </ul>

espress_stream_for_pico_w.20241214.tar.gz (1.6-1)
: <ul>
    <li>espress_streamのバージョンアップに対応した。</li>
  </ul>

espress_stream_for_pico_w.20240826.tar.gz (1.5-1)
: <ul>
    <li>乱数生成にpico_randを使用するようにした。</li>
  </ul>

espress_stream_for_pico_w.20240825.tar.gz (1.5-1)
: <ul>
    <li>ドキュメントの修正。</li>
    <li><tt>espress_stream_dgram.c</tt>を修正。</li>
  </ul>

espress_stream_for_pico_w.20240803.tar.gz (1.4-1)
: <ul>
    <li>espress_streamのバージョンアップに対応した。</li>
  </ul>

espress_stream_for_pico_w.20240704.tar.gz (1.3-0)
: <ul>
    <li>espress_streamのバージョンアップに対応した。</li>
  </ul>

espress_stream_for_pico_w.20240117.tar.gz (1.2-0)
: <ul>
    <li>espress_streamのバージョンアップに対応した。</li>
  </ul>

espress_stream_for_pico_w.20240115.tar.gz (1.1-0)
: <ul>
    <li>espress_streamのコードドキュメント変更を反映。</li>
  </ul>

espress_stream_for_pico_w.20240112.tar.gz (1.1-0)
: <ul>
    <li>espress_streamのバージョンアップに対応した。</li>
  </ul>

espress_stream_for_pico_w.20240110.tar.gz (1.0-0)
: <ul>
    <li>espress_streamのバージョンアップに対応した。</li>
  </ul>

espress_stream_for_pico_w.20240108.tar.gz (0.9-0)
: <ul>
    <li>ドキュメントの修正。</li>
  </ul>

espress_stream_for_pico_w.20240101.tar.gz (0.9-0)
: <ul>
    <li>設定値の一部が実験用の値のままになっていたので元の値に修正。</li>
    <li>ドキュメントの修正。</li>
  </ul>

espress_stream_for_pico_w.20231231.tar.gz (0.9-0)
: <ul>
    <li>初版。</li>
  </ul>

</details>

## 導入

### 準備

本ソフトウェアはRaspberry Pi Pico C/C++ SDKを用いてソースコードから作成します。
SDKの導入、ソフトウェア作成方法、SDKを用いて作成したソフトウェアのPico Wへの導入等の方法は[SDKのドキュメント](https://datasheets.raspberrypi.com/picow/connecting-to-the-internet-with-pico-w.pdf)を参照してください。

SDKを導入されているディレクトリ<tt>pico-sdk</tt>が存在するディレクトリでパッケージの解凍を行っておきます。

```
$ tar zxvf espress_stream_for_pico_w.########.tar.gz
```

パッケージには移植されたespress_streamとサンプルアプリケーションが含まれています。
サンプルアプリケーションは[linux用の<tt>es_ccat</tt>と<tt>es_ftx</tt>](http://akino-yo.f5.si/akinoyonika/espress_stream/#%E3%82%B5%E3%83%B3%E3%83%97%E3%83%AB%E3%82%A2%E3%83%97%E3%83%AA%E3%82%B1%E3%83%BC%E3%82%B7%E3%83%A7%E3%83%B3)を移植したものです。
<tt>es_ccat</tt>はクライアントアプリケーションで標準入力からの入力を接続したサーバーアプリケーションに送信し、
サーバーアプリケーションからの受信を標準出力に出力します。
<tt>es_ftx</tt>はクライアントアプリケーションで、サーバーアプリケーション<tt>es_frx</tt>に対してファイルを送信する機能を持ちます。
移植された<tt>es_ftx</tt>は<tt>test.dat</tt>というファイル名で1MBのダミーデータを送信するように作られています。

以下ではSDKの導入が完了していることを前提に、サンプルアプリケーションを動作させるためのバイナリーを作成するまでの手順を示します。

### バイナリーの作成

以下は<tt>es_ftx</tt>におけるバイナリー作成手順を示しています。<tt>es_ccat</tt>については適宜<tt>es_ftx</tt>部分を<tt>es_ccat</tt>に読み替えてください。

```
$ cd espress_stream_for_pico_w
$ vi net_config.h
$ mkdir build
$ cd build
$ PICO_SDK_PATH=../../pico-sdk PICO_BOARD=pico_w cmake ..
$ make es_ftx
```

<tt>api/include/net_config.h</tt>ではwifiアクセスポイントSSID、パスワードおよびサーバーのアドレス、ポート、公開鍵、ユーザーID、パスワードの設定を行っています。

```
#define WIFI_SSID "MY_WIFI_SSID"
#define WIFI_PASSWORD "my_wifi_password"
#define ADDRESS "192.168.1.2"
#define PORT "12345"
#define PUBLIC_KEY { \
	0x73, 0x35, 0xe6, 0x3a, 0x76, 0x21, 0x23, 0xa9, 0x41, 0xa2, 0x5e, 0x5e, 0x53, 0x8d, 0xd4, 0xd2, \
	0xe4, 0x7d, 0x86, 0x92, 0x80, 0xf8, 0x3a, 0xf7, 0x81, 0x0e, 0x0b, 0x3d, 0xae, 0x80, 0x69, 0xc9 \
}
#define USER_ID "foo"
#define USER_PASSWORD "tpfBTht8"
```

以上のような内容で作成してください。
公開鍵、ユーザーID、パスワードはサーバーの設定に合わせる必要があります。詳細はlinux用パッケージの該当ドキュメントを参照してください。
(<tt>ex_ccat</tt>の場合、ユーザーIDとパスワードの設定はありません。)

以上の手順で<tt>build</tt>ディレクトリに<tt>es_ftx.uf2</tt>が作成されます。
これをPico Wに導入してください。

### メイク時のワーニングについて

現在のところ、メイク時ワーニングが出ます。
同じコードはlinux用でもメイクされますが、ワーニングは出ておらず、問題ないと判断しています。

## 使用方法

### サンプルアプリケーション

#### <tt>es_ccat</tt>

linuxにてサーバーアプリケーション<tt>es_scat</tt>を起動してください。
また、Pico Wの起動後にusb経由でシリアルコンソールが接続するように準備しておいてください。(行末はLFで。)
30秒以内にソフトウェア導入済みのPico Wを起動すれば、少し時間をおいて"CONNECT"の表示がコンソールに出ます。
この状態でシリアルコンソールから文字列を入力するとlinux側にその文字列が出力されます。
同様にlinux側で文字列を入力するとシリアルコンソール側に文字列が出力されます。

#### <tt>es_ftx</tt>

linuxにてサーバーアプリケーション<tt>es_frx</tt>を起動、ファイル受信の準備をしておいてください。
ソフトウェア導入済みのPico Wを起動すればファイルの転送を開始します。
このとき、USB経由でシリアルコンソールを接続しておくと、ファイル転送プログレス等の情報が表示されます。

### espress_stream使用アプリケーション作成

<tt>espress_stream</tt>ディレクトリ以下をディレクトリごとRaspberry Pi Pico C/C++ SDKを用いたアプリケーション作成環境にコピーしてください。

<tt>CMakeLists.txt</tt>に設定を追加します。

<tt>include_directories</tt>
: <tt>espress_stream/api/include</tt>をインクルードパスに追加。

<tt>add_subdirectory</tt>
: <tt>espress_stream</tt>をサブディレクトリに追加。

<tt>target_link_libraries</tt>
: <tt>pico_stdlib</tt>、<tt>pico_cyw43_arch_lwip_threadsafe_background</tt><tt>espress_stream</tt>をライブラリーに追加。

UDPを使うため、<tt>pico_cyw43_arch_lwip_threadsafe_background</tt>をライブラリーに追加し、lwIPを導入しています。lwIPはオプションをlwipopts.hに設定します。これを作成し、<tt>api/include</tt>ディレクトリにおいてください。詳細は[pico-examplesの<tt>pico_w/wifi/lwipopts_examples_common.h</tt>](https://github.com/raspberrypi/pico-examples/blob/master/pico_w/wifi/lwipopts_examples_common.h)を参照してください。

アプリケーションからespress_streamを利用するためのAPIはlinux用ライブラリーと同じです。
詳細は「[linux用のライブラリー使用方法](http://akino-yo.f5.si/akinoyonika/espress_stream/#%E4%BD%BF%E7%94%A8%E6%96%B9%E6%B3%95)」を参照してください。

## ライセンス

MITライセンスに基づいてリリースされています。

<details><summary>Copyright 2023 akinoyonika</summary>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

</details>
