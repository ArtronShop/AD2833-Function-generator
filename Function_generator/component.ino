// Draw everything on screen

void drawBox(BoxType box, bool focus) {
  uint16_t color = focus ? tft.setColor(255, 255, 0) : tft.setColor(255, 255, 255);
  tft.setFont(Terminal6x8);
  
  switch (box) {
    case BOX_WAVEFROM: {
      // Waveform
      tft.drawRectangle(6, 12, 76, 170, color);
      tft.drawText(10, 8, "Waveform", color);
      break;
    }

    case BOX_FREQUENCY: {
      // Frequency
      tft.drawRectangle(80, 12, 214, 110, color);
      tft.drawText(84, 8, "Frequency", color);
      break;
    }

    case BOX_UNIT: {
      // Unit
      tft.drawRectangle(80, 122, 214, 170, color);
      tft.drawText(84, 118, "Unit", color);
      break;
    }
  }
}

void drawWaveIcon(WaveType type, bool focus) {
  int offsetY = type == WAVE_SINE ? 37 : type == WAVE_SQUARE ? 77 : 117;
  tft.fillRectangle(18, offsetY, 62, offsetY + 29, focus ? tft.setColor(0, 0, 255) : tft.setColor(0, 0, 0));
  
  switch (type) {
    case WAVE_SINE: {
      // draw Sin wave icon
      int x=21, y=52, oldX=21, oldY=52;
      for (int i=360;i>=0;i-=10) {
        x = x+1;
        y = 52 + sin(i * (22.0 / 7.0) / 180.0) * 10;
        tft.drawLine(oldX, oldY, x, y, tft.setColor(255, 255, 255));
        oldX = x;
        oldY = y;
      }
      break;
    }
    
    case WAVE_SQUARE: {
      // draw Square wave icon
      tft.drawLine(21, 102, 21, 82, tft.setColor(255, 255, 255));
      tft.drawLine(21, 82, 39, 82, tft.setColor(255, 255, 255));
      tft.drawLine(39, 82, 39, 102, tft.setColor(255, 255, 255));
      tft.drawLine(39, 102, 57, 102, tft.setColor(255, 255, 255));
      tft.drawLine(57, 102, 57, 82, tft.setColor(255, 255, 255));
      break;
    }
    
    case WAVE_TRIANGLE: {
      // draw Triangle wave
      tft.drawLine(21, 132, 30, 122, tft.setColor(255, 255, 255));
      tft.drawLine(30, 122, 48, 142, tft.setColor(255, 255, 255));
      tft.drawLine(48, 142, 57, 132, tft.setColor(255, 255, 255));
      break;
    }
  }
}

void drawUint(UnitType type, bool focus) {
  uint16_t color = focus ? tft.setColor(0, 0, 255) : tft.setColor(255, 255, 255);
  
  tft.setFont(Terminal11x16);
  switch(type) {
    case UINT_Hz: {
      tft.drawText(93, 140, "Hz", color);
      break;
    }

    case UINT_kHz: {
      tft.drawText(126, 140, "kHz", color);
      break;
    }

    case UINT_MHz: {
      tft.drawText(170, 140, "MHz", color);
      break;
    }
  }
}

void drawFreq(int n) {
  tft.fillRectangle(90, 40, 200, 90, tft.setColor(0, 0, 0));
  tft.setGFXFont(&FreeSans24pt7b);
  int w = 0, h = 0, x = 0, y = 0;
  tft.getGFXTextExtent(String(n), 0, 0, &w, &h);
  x = 147 - (w / 2);
  y = 40 + h;
  tft.drawGFXText(x, y, String(n), tft.setColor(255, 255, 255));
}
