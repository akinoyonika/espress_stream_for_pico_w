# Raspberry Pi Pico W用espress_stream

## 概要

[espress_stream](http://akino-yo.blue.coocan.jp/akinoyonika/espress_stream/)の[Raspberry Pi Pico W](https://www.raspberrypi.com/products/raspberry-pi-pico/)(以降Pico W)上への移植を行いました。
linux用とは以下のような違いがあります。

- シングルタスク、スレッド未使用。
- リソース固定、ヒープ未使用。
- 排他制御、同期機構関連部のダミー実装による無効化もしくは疑似動作化。
- lwIPによるデータグラム通信部。
- SDKのライブラリーによるティック生成部。
- ティックを利用した動作確認用の乱数生成部。

その他、小リソース向けに設定値を変更しています。

なお、開発環境には[Raspberry Pi Pico C/C++ SDK](https://datasheets.raspberrypi.com/picow/connecting-to-the-internet-with-pico-w.pdf)を用いました。
OSは未使用(ベア環境)です。

## パッケージ

本ソフトウェアはソースパッケージにて提供しています。以下をダウンロードしてください。

- [espress_stream_for_pico_w.20240115.tar.gz](http://akino-yo.blue.coocan.jp/akinoyonika/espress_stream_for_pico_w/espress_stream_for_pico_w.20240115.tar.gz)

<details><summary>変更履歴</summary>

espress_stream_for_pico_w.20240115.tar.gz (1.1-0)
: - espress_streamのコードドキュメント変更を反映。

espress_stream_for_pico_w.20240112.tar.gz (1.1-0)
: - espress_streamのバージョンアップに対応した。

espress_stream_for_pico_w.20240110.tar.gz (1.0-0)
: - espress_streamのバージョンアップに対応した。

espress_stream_for_pico_w.20240108.tar.gz (0.9-0)
: - ドキュメントの修正。

espress_stream_for_pico_w.20240101.tar.gz (0.9-0)
: - 設定値の一部が実験用の値のままになっていたので元の値に修正。
 - ドキュメントの修正。

espress_stream_for_pico_w.20231231.tar.gz (0.9-0)
: - 初版。

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
サンプルアプリケーションは[linux用の<tt>es_ftx</tt>](http://akino-yo.blue.coocan.jp/akinoyonika/espress_stream/#%E3%82%B5%E3%83%B3%E3%83%97%E3%83%AB%E3%82%A2%E3%83%97%E3%83%AA%E3%82%B1%E3%83%BC%E3%82%B7%E3%83%A7%E3%83%B3-es_ftx-es_frx)を移植したものです。
<tt>es_ftx</tt>はクライアントアプリケーションで、サーバーアプリケーション<tt>es_frx</tt>に対してファイルを送信する機能を持ちます。
移植された<tt>es_ftx</tt>は<tt>test.dat</tt>というファイル名で1MBのダミーデータを送信するように作られています。

以下ではSDKの導入が完了していることを前提に、サンプルアプリケーションを動作させるためのバイナリーを作成するまでの手順を示します。

### バイナリーの作成

```
$ cd espress_stream_for_pico_w
$ vi wifi_config.h
$ vi app_main.c
$ mkdir build
$ cd build
$ PICO_SDK_PATH=../../pico-sdk PICO_BOARD=pico_w cmake ..
$ make espress_stream_app_lwip_background
```

<tt>wifi_config.h</tt>ではwifiアクセスポイントの設定を行っています。

```
#define WIFI_SSID "MY_WIFI_SSID"
#define WIFI_PASSWORD "my_wifi_password"
```

以上のような内容で作成してください。

<tt>app_main.c</tt>ではサーバーのアドレス、ポート、公開鍵およびユーザーID、パスワードを設定していますので、この部分を編集します。

```
#define ADDRESS "192.168.1.2"
#define PORT "12345"
#define PUBLIC_KEY { \
	0x73, 0x35, 0xe6, 0x3a, 0x76, 0x21, 0x23, 0xa9, 0x41, 0xa2, 0x5e, 0x5e, 0x53, 0x8d, 0xd4, 0xd2, \
	0xe4, 0x7d, 0x86, 0x92, 0x80, 0xf8, 0x3a, 0xf7, 0x81, 0x0e, 0x0b, 0x3d, 0xae, 0x80, 0x69, 0xc9 \
}
#define USER_ID "foo"
#define USER_PASSWORD "tpfBTht8"
```

公開鍵、ユーザーID、パスワードは<tt>es_frx</tt>の設定に合わせる必要があります。詳細は「[linux用の<tt>es_ftx</tt>、<tt>es_frx</tt>](http://akino-yo.blue.coocan.jp/akinoyonika/espress_stream/#%E3%82%B5%E3%83%B3%E3%83%97%E3%83%AB%E3%82%A2%E3%83%97%E3%83%AA%E3%82%B1%E3%83%BC%E3%82%B7%E3%83%A7%E3%83%B3-es_ftx-es_frx)」を参照してください。

以上の手順で<tt>build</tt>ディレクトリに<tt>espress_stream_app_lwip_background.uf2</tt>が作成されます。
これをPico Wに導入してください。

### メイク時のワーニングについて

現在のところ、メイク時ワーニングが出ます。
同じコードはlinux用でもメイクされますが、ワーニングは出ておらず、問題ないと判断しています。

### 乱数生成部について

乱数生成部はespress_streamライブラリー(<tt>espress_stream_rand.c</tt>)とアプリケーションプログラム(<tt>app_main.c</tt>内<tt>getrandom</tt>関数)に存在します。
この部分は動作確認用の簡易な作りになっており、実使用には向きません。

## 使用方法

### サンプルアプリケーション

linuxにてサーバーアプリケーション<tt>es_frx</tt>を起動、ファイル受信の準備をしておいてください。
ソフトウェア導入済みのPico Wを起動すればファイルの転送を開始します。
このとき、USB経由でシリアルコンソールを接続しておくと、ファイル転送プログレス等の情報が表示されます。

### espress_stream使用アプリケーション作成

<tt>espress_stream</tt>ディレクトリ以下をディレクトリごとRaspberry Pi Pico C/C++ SDKを用いたアプリケーション作成環境にコピーしてください。

<tt>CMakeLists.txt</tt>に設定を追加します。

<tt>include_directories</tt>
: <tt>espress_stream/include</tt>、<tt>espress_stream/api/include</tt>をインクルードパスに追加。

<tt>add_executable</tt>
: <tt>espress_stream/src/\*.c</tt>、<tt>espress_stream/api/src/\*.c</tt>をソースに追加。

<tt>target_link_libraries</tt>
: <tt>pico_stdlib</tt>、<tt>pico_cyw43_arch_lwip_threadsafe_background</tt>をライブラリーに追加。

<tt>target_compile_definitions</tt>
: <tt>ESPRESS_STREAM_MAJOR_VERSION=#</tt>、<tt>ESPRESS_STREAM_MINOR_VERSION=#</tt>、<tt>ESPRESS_STREAM_PROTOCOL_VERSION=#</tt>を定義に追加。
"<tt>#</tt>"の部分は実際には数字です。
パッケージの<tt>CMakeLists.txt</tt>にある同設定のものに合わせてください。

UDPを使うため、<tt>pico_cyw43_arch_lwip_threadsafe_background</tt>をライブラリーに追加し、lwIPを導入しています。lwIPはオプションをlwipopts.hに設定します。これを作成し、いずれかのインクルードパスにおいてください。詳細は[pico-examplesの<tt>pico_w/wifi/lwipopts_examples_common.h</tt>](https://github.com/raspberrypi/pico-examples/blob/master/pico_w/wifi/lwipopts_examples_common.h)を参照してください。

アプリケーションからespress_streamを利用するためのAPIはlinux用ライブラリーと同じです。
詳細は「[linux用のライブラリー使用方法](http://akino-yo.blue.coocan.jp/akinoyonika/espress_stream/#%E3%83%A9%E3%82%A4%E3%83%96%E3%83%A9%E3%83%AA%E3%83%BC-libespress_stream.so)」を参照してください。

## ライセンス

MITライセンスに基づいてリリースされています。

<details><summary>Copyright 2023 akinoyonika</summary>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

</details>
