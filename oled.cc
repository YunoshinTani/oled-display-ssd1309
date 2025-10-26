/**
 * @file oled.cc
 * @author @YunoshinTani (taniyunoshin@gmail.com)
 * @since 2025/10/25
 * @date 2025/10/26
 * @version v1.0.0
*/

#include "oled.h"

extern const unsigned char (*GetFont5x8())[5];
const unsigned char (*OLED::font_)[5] = GetFont5x8();

/**
 * @brief コンストラクタ。
 * @details OLED(SSD1309)を定義する際に呼び出される。
 * @param sda I2CのSDAピン。
 * @param scl I2CのSCLピン。 
 * @param address I2Cのアドレス。(`デフォルト: 0x3C`)
 * @note Example: `OLED oled1(I2C_SDA, I2C_SCL);`
*/
OLED::OLED(PinName sda, PinName scl, uint8_t address) : i2c_(sda, scl) {
  address_ = address << 1;
}

/**
 * @brief コマンドを送信する。(プライベート メンバ関数)
 * @details SSD1309のコマンドを送信する。コマンドはデータシート参照。
 * @param cmd 送信するコマンド。
 * @return 送信が成功したか。成功したらtrue, 失敗したらfalseを返す。
*/
bool OLED::SendCommand(uint8_t cmd) {
  const char buf[2] = {0x00, cmd};
  return (i2c_.write(address_, buf, 2) == 0) ? true : false;
}

/**
 * @brief データを送信する。(プライベート メンバ関数)
 * @details SSD1309に表示内容のデータを送信する。SendCommandの後に使うことが多い。
 * @param data 送信するデータ。
 * @return 送信が成功したか。成功したらtrue, 失敗したらfalseを返す。
*/
bool OLED::SendData(uint8_t data) {
  const char buf[2] = {0x40, data};
  return (i2c_.write(address_, buf, 2) == 0) ? true : false;
}

/**
 * @brief OLEDディスプレイを初期化する。最初に実行すること。
 * @details SSD1309に初期設定のコマンドを複数送信する。
 * @note Example: `oled1.Init();`
*/
void OLED::Init() {
  i2c_.frequency(4e5);

  SendCommand(0xAE); // ディスプレイOFF
  SendCommand(0xD5); // ディスプレイクロック分周比/振動数設定
  SendCommand(0x80); // ディスプレイクロック分周比/振動数設定
  SendCommand(0xA8); // マルチプレクス比設定
  SendCommand(0x3F); // マルチプレクス比設定
  SendCommand(0xD3); // ディスプレイオフセット設定
  SendCommand(0x00); // ディスプレイオフセット設定
  SendCommand(0x40); // ディスプレイ開始行設定
  SendCommand(0x8D); // 充電ポンプ設定
  SendCommand(0x14); // 充電ポンプ設定
  SendCommand(0x20); // メモリアドレスモード設定
  SendCommand(0x00); // メモリアドレスモード設定
  SendCommand(0xA1); // セグメントリマップ設定
  SendCommand(0xC8); // コモンリマップ設定
  SendCommand(0xDA); // ハードウェアコンフィギュレーション設定
  SendCommand(0x12); // ハードウェアコンフィギュレーション設定
  SendCommand(0x81); // コントラスト設定
  SendCommand(0xCF); // コントラスト設定
  SendCommand(0xD9); // プリチャージ設定
  SendCommand(0xF1); // プリチャージ設定
  SendCommand(0xDB); // VCOMHデセレクトレベル設定
  SendCommand(0x40); // VCOMHデセレクトレベル設定
  SendCommand(0xA4); // 全画面表示ON（通常表示）
  SendCommand(0xA6); // 通常表示モード
  SendCommand(0xAF); // ディスプレイON
}

/**
 * @brief ディスプレイと通信できるか調べる。
 * @details SSD1309にディスプレイONのコマンドを送り、通信できるか調べる。
 * @return 送信が成功したか。成功したらtrue, 失敗したらfalse。
 * @note Example: `oled1.Debug();`
*/
bool OLED::Debug() {
  return SendCommand(0xAF);
}

/**
 * @brief ディスプレイのピクセルをすべて消す。
 * @details ディスプレイのすべてのピクセルをすべて0にするデータを送信する。
 * @note Example: `oled1.Clear();`
*/
void OLED::Clear() {
  for (uint8_t row=0; row<8; row++) { // row
    SetCursor(0, row);
    for (uint8_t column=0; column<128; column++) {
      SendData(0x00);
    }
  }
}

/**
 * @brief ディスプレイのピクセルをすべて点灯させる。
 * @details ディスプレイのピクセルをすべて1にするデータを送信する。
 * @note Example: `oled1.Fill();`
*/
void OLED::Fill() {
  for (uint8_t row=0; row<8; row++) { // row
    SetCursor(0, row);
    for (uint8_t column=0; column<128; column++) {
      SendData(0xFF);
    }
  }
}

/**
 * @brief カーソルのX座標と行を指定する。(プライベート メンバ関数)
 * @details 送信したデータが反映されるX座標と行を指定するコマンドを送信する。
 * @param x X座標。
 * @param row 行。
*/
void OLED::SetCursor(uint8_t x, uint8_t row) {
  SendCommand(0xB0 + row);
  SendCommand(0x00 + (x & 0x0F));
  SendCommand(0x10 + ((x >> 4) & 0x0F));
}

/**
 * @brief 点を表示させる。
 * @details 指定したX,Y座標に1ピクセルの点を表示させる。
 * @param x X座標。
 * @param y Y座標。
 * @note Example: `oled1.DrawPoint(72, 110);`
 * @warning 同じX座標かつ同じ行の点は上書きされるので注意。
*/
void OLED::DrawPoint(uint8_t x, uint8_t y) {
  SetCursor(x, y/8);
  SendData(0x01 << (y % 8));
}

/**
 * @brief 整数の数字を表示する。(X座標指定)
 * @details 整数の数字を、指定した行のX座標に表示する。DrawNum関数のオーバーロードの一つ。
 * @param num 表示する数字(int型)。
 * @param row 表示する行(0~7)。
 * @param x 表示する左端のX座標(0~127)。
 * @param option 数字の表示オプション。printf()内のオプションと同じように設定できる。(`デフォルト: "%d%`)
 * @note Example: `oled1.DrawInt(810, 4, 30, %05d);`
 * @note > `00810` が 4行目、X座標30 から表示される。
*/
void OLED::DrawInt(int num, uint8_t row, uint8_t x, const char *option) {
  char buf[10];
  sprintf(buf, option, num);
  DrawText(buf, row, x);
}

/**
 * @brief 整数の数字を表示する。(揃え指定)
 * @details 整数の数字を、指定した方法で揃え、表示する。DrawNum関数のオーバーロードの一つ。
 * @param num 表示する数字(int型)。
 * @param row 表示する行(0~7)。
 * @param align どこで揃えるか。Align::Left, Center, Right から選べる。
 * @param option 数字の表示オプション。printf()内のオプションと同じように設定できる。(`デフォルト: "%d"`)
 * @note Example: `oled1.DrawInt(810, 4, Align::Center, %05d);`
 * @note > `00810` が 4行目の中央 に表示される。
*/
void OLED::DrawInt(int num, uint8_t row, Align align, const char *option) {
  char buf[10];
  sprintf(buf, option, num);
  DrawText(buf, row, align);
}

/**
 * @brief 指定した文字のフォントを出力する。(プライベート メンバ関数)
 * @details SSD1309に指定したchar型(8ビット)のデータに対応するフォントを表示するデータを送る。
 * @param data テキスト(1文字)。
 * @param reserve 白黒を反転するか。(`デフォルト: false`)
*/
void OLED::DrawChar(char data, bool reserve) {
  for (uint8_t i=0; i<5; i++) {
    if (reserve) SendData(~(font_[data - 32][i]));
    else SendData(font_[data - 32][i]);
  }
  if (reserve) SendData(0xFF);
  else SendData(0x00);
}

/**
 * @brief テキストを表示する。(X座標指定)
 * @details 入力した文字列を表示する。DrawText関数のオーバーロードの一つ。
 * @param text 文字列。
 * @param row 表示する行(0~7)。
 * @param x 表示する左端のX座標(0~127)。
 * @param reserve 白黒を反転するか。(`デフォルト: false`)
 * @note Example: `DrawText("Hello, World!", 4, 20, false)`
 * @note > `Hello, World!` が 4行目、X座標20 から表示される。
*/
void OLED::DrawText(const char *text, uint8_t row, uint8_t x, bool reserve) {
  SetCursor(x, row);
  if (reserve) SendData(0xFF);
  else SendData(0x00);
  while (*text) {
    DrawChar(*text++, reserve);
  }
}

/**
 * @brief テキストを表示する。(揃え指定)
 * @details 入力した文字列を表示する。DrawText関数のオーバーロードの一つ。
 * @param text 文字列。
 * @param row 表示する行(0~7)。
 * @param align どこで揃えるか。Align::Left, Center, Right から選べる。(`デフォルト: Align::Left`)
 * @param reserve 白黒を反転するか。(`デフォルト: false`)
 * @note Example: `DrawText("Hello, World!", 4, Center, false)`
 * @note > `Hello, World!` が 4行目の中央 に表示される。
*/
void OLED::DrawText(const char *text, uint8_t row, Align align, bool reserve) {
  if (align == Align::Left) SetCursor(0, row);
  else if (align == Align::Center) SetCursor((128-strlen(text)*6)/2, row);
  else if (align == Align::Right) SetCursor(128-strlen(text)*6, row);
  else SetCursor(0, row);
  if (reserve) SendData(0xFF);
  else SendData(0x00);
  while (*text) {
    DrawChar(*text++, reserve);
  }
}

/**
 * @brief 入力したデータをそのまま表示する。画像などを表示するときに使う。
 * @details SSD1309に、入力した配列の内容のデータをそのまま送信する。
 * @param data 表示するデータの2次元配列。
 * @param row 表示するデータの上端の行(0~7)。(`デフォルト: 0`)
 * @param x 表示するデータの左端のX座標(0~127)。(`デフォルト: 0`)
 * @param max_row 表示するデータの行の数。(`デフォルト: 8`)
 * @param max_column 表示するデータのX座標の幅。(`デフォルト: 16`)
 * @note Example: oled1.DrawData(logo_data);
 * @note > ロゴのデータが画面全体に表示される。(デフォルト引数の使用)
*/
void OLED::DrawData(const uint8_t data[][8], uint8_t row, uint8_t x, uint8_t max_row, uint8_t max_column) {
  for (uint8_t r=0; r<max_row; r++) {
    for (uint8_t column=0; column<max_column; column++) {
      SetCursor(column*8 + x, r + row);
      for (uint8_t i=0; i<8; i++) {
        SendData(data[r*max_column + column][i]);
      }
    }
  }
}

/**
 * @brief QRコードを表示する。
 * @details QRコードのデータを渡すと適切な幅で表示される。
 * @param data QRコードのデータの二次元配列。
 * @param row QRコードの上端の行(0~7)。
 * @param x QRコードの左端のX座標(0~127)。
*/
void OLED::DrawQR(const uint8_t data[25][8], uint8_t row, uint8_t x) {
  DrawData(data, x, row, 5, 5);
}
