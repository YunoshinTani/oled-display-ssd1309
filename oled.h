/**
 * @file oled.h
 * @author @YunoshinTani (taniyunoshin@gmail.com)
 * @since 2025/10/25
 * @date 2025/10/26
 * @version v1.0.0
*/

#ifndef OLED_DISPLAY_SSD1309_OLED_H_
#define OLED_DISPLAY_SSD1309_OLED_H_

#include <PinNames.h>
#include <I2C.h>

enum class Align {
  Left,
  Center,
  Right,
};

class OLED {
 public:
  OLED(PinName sda, PinName scl, uint8_t address = 0x3C);

  void Init();

  bool Debug();

  void Clear();

  void Fill();

  void DrawPoint(uint8_t x, uint8_t y);

  void DrawInt(int num, uint8_t row, uint8_t x = 0, const char *option = "%d");
  void DrawInt(int num, uint8_t row, Align align, const char *option = "%d");
  
  void DrawText(const char *text, uint8_t row, uint8_t x, bool reserve = false);
  void DrawText(const char *text, uint8_t row, Align align = Align::Left, bool reserve = false);
  
  void DrawData(const uint8_t data[][8], uint8_t row = 0 , uint8_t x = 0, uint8_t max_row = 8, uint8_t max_column = 16);
  
  void DrawQR(const uint8_t data[25][8], uint8_t row, uint8_t x);
  
  private:
  bool SendCommand(uint8_t cmd);
  bool SendData(uint8_t data);
  void SetCursor(uint8_t x, uint8_t y);
  void DrawChar(char data, bool reserve = false);
  mbed::I2C i2c_;
  uint8_t address_;
  static const unsigned char (*font_)[5];
};

#endif // OLED_DISPLAY_SSD1309_OLED_H_
