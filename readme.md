
# OLED Display SSD1309 ライブラリ

このライブラリは mbed 環境で SSD1309 ベースの 128x64（または互換）OLED ディスプレイを簡単に扱うための C++ ライブラリです。I2C 接続を使って描画コマンドやデータを送信します。  
Gitリポジトリ : [oled-display-ssd1309](https://github.com/YunoshinTani/oled-display-ssd1309.git)  

## 概要

- コンストラクタで SDA, SCL のピンを指定してインスタンス化します。  
- 初期化、クリア、描画（テキスト、整数、点、QR、バイナリデータ）などの基本機能を提供します。  
- 内部で mbed::I2C を使用しており、mbed ベースプロジェクトでの利用が想定されています。  

## 対応ハードウェア

- SSD1309（I2C）互換の OLED モジュール  
- ほとんどの 3.3V MCU（mbed 対応ボード）  

注: 使用するボードや OLED モジュールの電源電圧（3.3V/5V）はハードウェアに合わせて確認してください。  

## 配線（一般例、I2C）

- OLED SDA -> MCU SDA ピン（コンストラクタで指定）  
- OLED SCL -> MCU SCL ピン（コンストラクタで指定）  
- OLED VCC -> MCU 3.3V（モジュールに合わせる）  
- OLED GND -> MCU GND  

必要に応じてプルアップ抵抗を確認してください（多くのモジュールには実装済みです）。  

## 使い方（簡単な例）

以下は基本的な使用例です。ピン名はボードに合わせて変更してください。  

```cpp
#include <mbed.h>
#include <oled.h>

// 例: SDA, SCL をボード固有の PinName に置き換えてください
OLED oled(PB_9, PB_8); // (例) SDA, SCL

int main() {
	oled.Init();        // ディスプレイ初期化
	oled.Clear();       // 画面クリア
	oled.DrawText("Hello, World!", 4, Align::Center); // row=0, 中央 に描画
	oled.DrawInt(12345, 1, 50);               // row=1, x=50 に整数を描画
}
```

上記は最小限の例です。ピン名やスレッド API はプラットフォーム（mbed OS のバージョン）に合わせて調整してください。  

## API 概要

このライブラリが提供する主なメソッドは次のとおりです。  

- OLED(PinName sda, PinName scl, uint8_t address = 0x3C)
	- コンストラクタ。I2C の SDA/SCL ピンと I2C アドレスを設定します（デフォルト 0x3C）。
- void Init()
	- ディスプレイの初期化と設定を行います。
- bool Debug()
	- デバッグ用フラグまたは状態を返します（内部実装に依存）。
- void Clear()
	- 画面を全消去します。
- void Fill()
	- 画面を全点灯します。
- void DrawPoint(uint8_t x, uint8_t y)
	- (x,y) に点を描画します。
- void DrawInt(int num, uint8_t row, uint8_t x = 0, const char *option = "%d")
	- 指定行に整数を描画します。書式は printf 形式で指定できます。
- void DrawInt(int num, uint8_t row, Align align, const char *option = "%d")
	- 行揃え（左/中央/右）を指定して整数を描画します。
- void DrawText(const char *text, uint8_t row, uint8_t x, bool reserve = false)
	- 指定位置にテキストを描画します。
- void DrawText(const char *text, uint8_t row, Align align = Align::Left, bool reserve = false)
	- 行揃え（左/中央/右）を指定してテキストを描画します。
- void DrawData(const uint8_t data[][8], uint8_t row = 0 , uint8_t x = 0, uint8_t max_row = 8, uint8_t max_column = 16)
	- バイナリデータ（フォントやビットマップ）を直接描画します。
- void DrawQR(const uint8_t data[25][8], uint8_t row, uint8_t x)
	- QR コードデータ（特定フォーマット）を描画します。

詳細な振る舞いや引数の制約はヘッダ（`oled.h`）を参照してください。  

## フォント/データ形式

ライブラリは内部にフォントテーブルを持っています（`font.cc`）。カスタムビットマップを描画する場合は `DrawData` の期待フォーマットに従ってください（1 バイト = 8 ピクセル縦、行列指定で描画）。  

## 注意点 / エッジケース

- I2C アドレスはモジュールによって異なる場合があります（0x3C/0x3D 等）。接続前に確認してください。  
- ディスプレイサイズ（列・行）にライブラリが正しく対応しているか確認してください。  
- 大きな描画更新を行う場合は適切なタイミング制御や画面更新の間隔を考慮してください。  

## 作者 / 連絡先
@YunoshinTani  
taniyunoshin@gmail.com  

---
