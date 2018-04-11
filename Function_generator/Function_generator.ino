/* Developed by IOXhop : www.ioxhop.com */

#include<SPI.h>
#include<TFT_22_ILI9225.h>
#include <../fonts/FreeSans24pt7b.h>
#include <i2c_keypad.h>

// LCD Connect pin
#define TFT_RST 8
#define TFT_RS  9
#define TFT_CS  10  // SS
// #define TFT_SDI 11  // MOSI
// #define TFT_CLK 13  // SCK
#define TFT_LED 3   // 0 if wired to +5V directly

// AD9833 Connect pin
#define FSYNC 4

enum BoxType {
  BOX_WAVEFROM,
  BOX_FREQUENCY,
  BOX_UNIT
};

enum WaveType {
  WAVE_SINE = 0x2000,
  WAVE_SQUARE = 0x2028,
  WAVE_TRIANGLE = 0x2002
};

enum UnitType {
  UINT_Hz = 0,
  UINT_kHz = 3,
  UINT_MHz = 6
};

extern void drawBox(BoxType box, bool focus = false) ;
extern void drawWaveIcon(WaveType type, bool focus = false) ;
extern void drawUint(UnitType type, bool focus = false) ;
extern void drawFreq(int n) ;
extern void AD9833setup() ;
extern void AD9833setFrequency(long frequency, int Waveform) ;
extern void WriteRegister(int dat) ;

WaveType wave = WAVE_SINE;
unsigned int feq = 0;
UnitType uint = UINT_Hz;

String TempFeq = "";
BoxType BoxFocus = BOX_WAVEFROM;

TFT_22_ILI9225 tft(TFT_RST, TFT_RS, TFT_CS, TFT_LED, 200);
I2CKEYPAD key;

void setup() {
  // Serial.begin(9600);

  AD9833setup();
  
  key.begin(0x20, 100);
  key.on(PRESS, [](char key) {
    // Serial.print("PRESS: ");
    // Serial.println(key);

    if (key >= 'A' && key <= 'C') {
      drawBox(BoxFocus, false); // Old focus set to not focus
      BoxFocus = key - 'A';
      drawBox(BoxFocus, true);
      if (BoxFocus == BOX_FREQUENCY) {
        TempFeq = "";
      }
    }

    if (key >= '0' && key <= '9') {
      if (BoxFocus == BOX_WAVEFROM) {
        if (key >= '1' && key <= '3') {
          drawWaveIcon(wave, false);
          wave = key == '1' ? WAVE_SINE : key == '2' ? WAVE_SQUARE : WAVE_TRIANGLE;
          drawWaveIcon(wave, true);
        }
      } else if (BoxFocus == BOX_FREQUENCY) {
        if (key >= '0' && key <= '9') {
          if ((TempFeq.length() == 0 && key != '0') || (TempFeq.length() > 0 && TempFeq.length() < 3)) {
            TempFeq += key;
          }
          feq = TempFeq.toInt();
          if (uint == UINT_MHz && feq > 12) feq = 12;
          drawFreq(feq);
        }
      } else if (BoxFocus == BOX_UNIT) {
        if (key >= '1' && key <= '3') {
          drawUint(uint, false);
          uint = key == '1' ? UINT_Hz : key == '2' ? UINT_kHz : UINT_MHz;
          drawUint(uint, true);
          if (uint == UINT_MHz && feq > 12) {
            feq = 12;
            drawFreq(feq);
          }
        }
      }
      AD9833setFrequency(feq * pow(10, uint), wave);
    }
  });
  
  tft.begin();

  tft.setOrientation(3);
  tft.fillRectangle(0, 0, tft.maxX()-1, tft.maxY()-1, tft.setColor(0, 0, 0));

  drawBox(BOX_WAVEFROM, true);
  drawWaveIcon(WAVE_SINE, true);
  drawWaveIcon(WAVE_SQUARE);
  drawWaveIcon(WAVE_TRIANGLE);
  
  drawBox(BOX_FREQUENCY);
  drawFreq(0);
  
  drawBox(BOX_UNIT);
  drawUint(UINT_Hz, true);
  drawUint(UINT_kHz);
  drawUint(UINT_MHz);
}

void loop() {
  key.scand();
}

