#include "hx8347lib.h"
#include "glcdfont.c"
#include <avr/pgmspace.h>
#include "pins_arduino.h"
#include "wiring_private.h"

  uint16_t rotation;
  static const uint16_t TFTHEIGHT = 240;
  static const uint16_t TFTWIDTH = 320;

// draw a rectangle
void HX8347::Rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, 
		      uint16_t color) {
  // smarter version
  H_line(x, y, w, color);
  H_line(x, y+h-1, w, color);
  V_line(x, y, h, color);
  V_line(x+w-1, y, h, color);
}

void HX8347::Lcd_Writ_Bus(unsigned char VH)
{
  unsigned int i,temp,data; 
  data=VH;
  for(i=8;i<=9;i++)
  {
    temp=(data&0x01);
    if(temp)
      digitalWrite(i,HIGH);
    else
      digitalWrite(i,LOW);
    data=data>>1;
  }	
  for(i=2;i<=7;i++)
  {
    temp=(data&0x01);
    if(temp)
      digitalWrite(i,HIGH);
    else
      digitalWrite(i,LOW);
    data=data>>1;
  }	 

  digitalWrite(_wr,LOW);
  digitalWrite(_wr,HIGH);
}


void HX8347::Lcd_Write_Com(unsigned char VH)  
{   
  digitalWrite(_cd,LOW);
  Lcd_Writ_Bus(VH);
}

void HX8347::Lcd_Write_Data(unsigned char VH)
{
  digitalWrite(_cd,HIGH);
  Lcd_Writ_Bus(VH);
}

void HX8347::Lcd_Write_Com_Data(unsigned char com,unsigned char dat)
{
  Lcd_Write_Com(com);
  Lcd_Write_Data(dat);
}

void HX8347::Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
        Lcd_Write_Com_Data(0x02,x1>>8);
	Lcd_Write_Com_Data(0x03,x1);
	Lcd_Write_Com_Data(0x04,x2>>8);
	Lcd_Write_Com_Data(0x05,x2);
	Lcd_Write_Com_Data(0x06,y1>>8);
	Lcd_Write_Com_Data(0x07,y1);
	Lcd_Write_Com_Data(0x08,y2>>8);
	Lcd_Write_Com_Data(0x09,y2);
	Lcd_Write_Com(0x22); 							 
}

void HX8347::reset(void) {
  if (_reset)
  digitalWrite(_reset, LOW);
  delay(2); 
  if (_reset)
    digitalWrite(_reset, HIGH);

  // resync
  Lcd_Write_Data(0);
  Lcd_Write_Data(0);
  Lcd_Write_Data(0);
  Lcd_Write_Data(0);
}


void HX8347::Lcd_Init(void)
{
/*  digitalWrite(_reset,HIGH);
  delay(5); 
  digitalWrite(_reset,LOW);
  delay(15);
  digitalWrite(_reset,HIGH);
  delay(15);
*/

reset();

  digitalWrite(_cs,HIGH);
  digitalWrite(_wr,HIGH);
  digitalWrite(_cs,LOW);  //CS

    Lcd_Write_Com_Data(0x46,0x91);
	Lcd_Write_Com_Data(0x47,0x11);
	Lcd_Write_Com_Data(0x48,0x00);
	Lcd_Write_Com_Data(0x49,0x66);
	Lcd_Write_Com_Data(0x4a,0x37);
	Lcd_Write_Com_Data(0x4b,0x04);
	Lcd_Write_Com_Data(0x4c,0x11);
	Lcd_Write_Com_Data(0x4d,0x77);
	Lcd_Write_Com_Data(0x4e,0x00);
	Lcd_Write_Com_Data(0x4f,0x1f);
	Lcd_Write_Com_Data(0x50,0x0f);
	Lcd_Write_Com_Data(0x51,0x00);

	//240x320 window setting
	Lcd_Write_Com_Data(0x02,0x00); // Column address start2
	Lcd_Write_Com_Data(0x03,0x00); // Column address start1
	Lcd_Write_Com_Data(0x04,0x00); // Column address end2
	Lcd_Write_Com_Data(0x05,0xef); // Column address end1
	Lcd_Write_Com_Data(0x06,0x00); // Row address start2
	Lcd_Write_Com_Data(0x07,0x00); // Row address start1
	Lcd_Write_Com_Data(0x08,0x01); // Row address end2
	Lcd_Write_Com_Data(0x09,0x3f); // Row address end1

	// Display Setting
	Lcd_Write_Com_Data(0x01,0x06); // IDMON=0, INVON=1, NORON=1, PTLON=0
	Lcd_Write_Com_Data(0x16,0xC0); // MY=0, MX=0, MV=0, ML=1, BGR=0, TEON=0   0048
	Lcd_Write_Com_Data(0x23,0x95); // N_DC=1001 0101
	Lcd_Write_Com_Data(0x24,0x95); // PI_DC=1001 0101
	Lcd_Write_Com_Data(0x25,0xFF); // I_DC=1111 1111

	Lcd_Write_Com_Data(0x27,0x02); // N_BP=0000 0010
	Lcd_Write_Com_Data(0x28,0x02); // N_FP=0000 0010
	Lcd_Write_Com_Data(0x29,0x02); // PI_BP=0000 0010
	Lcd_Write_Com_Data(0x2a,0x02); // PI_FP=0000 0010
	Lcd_Write_Com_Data(0x2C,0x02); // I_BP=0000 0010
	Lcd_Write_Com_Data(0x2d,0x02); // I_FP=0000 0010

	Lcd_Write_Com_Data(0x3a,0x01); // N_RTN=0000, N_NW=001    0001
	Lcd_Write_Com_Data(0x3b,0x00); // P_RTN=0000, P_NW=001
	Lcd_Write_Com_Data(0x3c,0xf0); // I_RTN=1111, I_NW=000
	Lcd_Write_Com_Data(0x3d,0x00); // DIV=00
	delay(1);
	Lcd_Write_Com_Data(0x35,0x38); // EQS=38h
	Lcd_Write_Com_Data(0x36,0x78); // EQP=78h
	Lcd_Write_Com_Data(0x3E,0x38); // SON=38h
	Lcd_Write_Com_Data(0x40,0x0F); // GDON=0Fh
	Lcd_Write_Com_Data(0x41,0xF0); // GDOFF

	// Power Supply Setting
	Lcd_Write_Com_Data(0x19,0x49); // CADJ=0100, CUADJ=100, OSD_EN=1 ,60Hz
	Lcd_Write_Com_Data(0x93,0x0F); // RADJ=1111, 100%
	delay(1);
	Lcd_Write_Com_Data(0x20,0x40); // BT=0100
	Lcd_Write_Com_Data(0x1D,0x07); // VC1=111   0007
	Lcd_Write_Com_Data(0x1E,0x00); // VC3=000
	Lcd_Write_Com_Data(0x1F,0x04); // VRH=0011

	//VCOM SETTING
	Lcd_Write_Com_Data(0x44,0x4D); // VCM=101 0000  4D
	Lcd_Write_Com_Data(0x45,0x0E); // VDV=1 0001   0011
	delay(1);
	Lcd_Write_Com_Data(0x1C,0x04); // AP=100
	delay(2);

	Lcd_Write_Com_Data(0x1B,0x18); // GASENB=0, PON=0, DK=1, XDK=0, VLCD_TRI=0, STB=0
	delay(1);
	Lcd_Write_Com_Data(0x1B,0x10); // GASENB=0, PON=1, DK=0, XDK=0, VLCD_TRI=0, STB=0
	delay(1);
	Lcd_Write_Com_Data(0x43,0x80); //set VCOMG=1
	delay(2);

	// Display ON Setting
	Lcd_Write_Com_Data(0x90,0x7F); // SAP=0111 1111
	Lcd_Write_Com_Data(0x26,0x04); //GON=0, DTE=0, D=01
	delay(1);
	Lcd_Write_Com_Data(0x26,0x24); //GON=1, DTE=0, D=01
	Lcd_Write_Com_Data(0x26,0x2C); //GON=1, DTE=0, D=11
	delay(1);
	Lcd_Write_Com_Data(0x26,0x3C); //GON=1, DTE=1, D=11

	// INTERNAL REGISTER SETTING
	Lcd_Write_Com_Data(0x57,0x02); // TEST_Mode=1: into TEST mode
	Lcd_Write_Com_Data(0x95,0x01); // SET DISPLAY CLOCK AND PUMPING CLOCK TO SYNCHRONIZE
	Lcd_Write_Com_Data(0x57,0x00); // TEST_Mode=0: exit TEST mode
	Lcd_Write_Com_Data(0x21,0x00);
	Lcd_Write_Com(0x22);   
    digitalWrite(_cs,HIGH);
}

void HX8347::H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c)                   
{	
  unsigned int i,j;
  Lcd_Write_Com(0x02c); //write_memory_start
  digitalWrite(_cd,HIGH);
  digitalWrite(_cs,LOW);
  l=l+x;
  Address_set(x,y,l,y);
  j=l*2;
  for(i=1;i<=j;i++)
  {
    Lcd_Write_Data(c);
  }
  digitalWrite(_cs,HIGH);   
}

void HX8347::V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c)                   
{	
  unsigned int i,j;
  Lcd_Write_Com(0x02c); //write_memory_start
  digitalWrite(_cd,HIGH);
  digitalWrite(_cs,LOW);
  l=l+y;
  Address_set(x,y,x,l);
  j=l*2;
  for(i=1;i<=j;i++)
  { 
    Lcd_Write_Data(c);
  }
  digitalWrite(_cs,HIGH);   
}

void HX8347::Rectf(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int c)
{
  unsigned int i;
  for(i=0;i<h;i++)
  {
    H_line(x  , y  , w, c);
    H_line(x  , y+i, w, c);
  }
}

// draw a character
void HX8347::drawChar(uint16_t x, uint16_t y, char c, 
		      uint16_t color, uint8_t size) {
  for (uint8_t i =0; i<5; i++ ) {
    uint8_t line = pgm_read_byte(font+(c*5)+i);
    for (uint8_t j = 0; j<8; j++) {
      if (line & 0x1) {
	if (size == 1) // default size
	  drawPixel(x+i, y+j, color);
	else {  // big size
	  Rectf(x+i*size, y+j*size, size, size, color);
	} 
      }
      line >>= 1;
    }
  }
}

void HX8347::drawString(uint16_t x, uint16_t y, char *c, uint16_t color, uint8_t size) 
{
  while (c[0] != 0) {
    drawChar(x, y, c[0], color, size);
    x += size*6;
    c++;
  }
}

void HX8347::drawCircle(uint16_t x0, uint16_t y0, uint16_t r, 
			uint16_t color) {
  drawPixel(x0, y0+r, color);
  drawPixel(x0, y0-r, color);
  drawPixel(x0+r, y0, color);
  drawPixel(x0-r, y0, color);

  drawCircleHelper(x0, y0, r, 0xF, color);
}

void HX8347::drawCircleHelper(uint16_t x0, uint16_t y0, uint16_t r, uint8_t cornername,
			uint16_t color) {
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;


  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    if (cornername & 0x4) {
      drawPixel(x0 + x, y0 + y, color);
      drawPixel(x0 + y, y0 + x, color);
    } 
    if (cornername & 0x2) {
      drawPixel(x0 + x, y0 - y, color);
      drawPixel(x0 + y, y0 - x, color);
    }
    if (cornername & 0x8) {
      drawPixel(x0 - y, y0 + x, color);
      drawPixel(x0 - x, y0 + y, color);
    }
    if (cornername & 0x1) {
      drawPixel(x0 - y, y0 - x, color);
      drawPixel(x0 - x, y0 - y, color);
    }
  }
}

void HX8347::drawPixel(uint16_t x, uint16_t y, uint16_t color)
{
  // check rotation, move pixel around if necessary
  Serial.println("In drawPixel");
  switch (rotation) {
  case 1:
    swap(x, y);
    x = TFTWIDTH - x - 1;
    break;
  case 2:
    x = TFTWIDTH - x - 1;
    y = TFTHEIGHT - y - 1;
    break;
  case 3:
    swap(x, y);
    y = TFTHEIGHT - y - 1;
    break;
  }
  
  Serial.print("X "); Serial.println(x);
  Serial.print("TFTWIDTH "); Serial.println(TFTWIDTH);
  Serial.print("Y "); Serial.println(y);
  Serial.print("TFTHEIGHT "); Serial.println(TFTHEIGHT);  
  if ((x >= TFTWIDTH) || (y >= TFTHEIGHT)) return;
  
  Address_set(x,y,x,y);
  //Lcd_Write_Com_Data(TFTLCD_GRAM_HOR_AD, x); // GRAM Address Set (Horizontal Address) (R20h)
  //Lcd_Write_Com_Data(TFTLCD_GRAM_VER_AD, y); // GRAM Address Set (Vertical Address) (R21h)
  //Lcd_Write_Com(TFTLCD_RW_GRAM);  // Write Data to GRAM (R22h)
  Lcd_Write_Data(color);
}

void HX8347::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, 
		      uint16_t color) {
  // if you're in rotation 1 or 3, we need to swap the X and Y's

  Lcd_Write_Com(0x02c); //write_memory_start
  digitalWrite(_cd,HIGH);
  digitalWrite(_cs,LOW);
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  Serial.print("abs y "); Serial.println(abs(y1-y0));
  Serial.print("steep "); Serial.println(steep);
  if (steep) {
    swap(x0, y0);
    swap(x1, y1);
  }

  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  //dy = abs(y1 - y0);
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;}

  for (; x0<=x1; x0++) {
    if (steep) {
      Serial.print("y0 "); Serial.println(y0);
      Serial.print("x0 "); Serial.println(x0);
      drawPixel(y0, x0, color);
      delay(5);
    } else {
      drawPixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
  digitalWrite(_cs,HIGH);
}

void HX8347::drawTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
  drawLine(x0, y0, x1, y1, color);
  drawLine(x1, y1, x2, y2, color);
  drawLine(x2, y2, x0, y0, color); 
}

void HX8347::fillTriangle ( int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint16_t color)
{
  if (y0 > y1) {
    swap(y0, y1); swap(x0, x1);
  }
  if (y1 > y2) {
    swap(y2, y1); swap(x2, x1);
  }
  if (y0 > y1) {
    swap(y0, y1); swap(x0, x1);
  }

  int32_t dx1, dx2, dx3; // Interpolation deltas
  int32_t sx1, sx2, sy; // Scanline co-ordinates

  sx2=(int32_t)x0 * (int32_t)1000; // Use fixed point math for x axis values
  sx1 = sx2;
  sy=y0;

  // Calculate interpolation deltas
  if (y1-y0 > 0) dx1=((x1-x0)*1000)/(y1-y0);
    else dx1=0;
  if (y2-y0 > 0) dx2=((x2-x0)*1000)/(y2-y0);
    else dx2=0;
  if (y2-y1 > 0) dx3=((x2-x1)*1000)/(y2-y1);
    else dx3=0;

  // Render scanlines (horizontal lines are the fastest rendering method)
  if (dx1 > dx2)
  {
    for(; sy<=y1; sy++, sx1+=dx2, sx2+=dx1)
    {
      H_line(sx1/1000, sy, (sx2-sx1)/1000, color);
    }
    sx2 = x1*1000;
    sy = y1;
    for(; sy<=y2; sy++, sx1+=dx2, sx2+=dx3)
    {
      H_line(sx1/1000, sy, (sx2-sx1)/1000, color);
    }
  }
  else
  {
    for(; sy<=y1; sy++, sx1+=dx1, sx2+=dx2)
    {
      H_line(sx1/1000, sy, (sx2-sx1)/1000, color);
    }
    sx1 = x1*1000;
    sy = y1;
    for(; sy<=y2; sy++, sx1+=dx3, sx2+=dx2)
    {
      H_line(sx1/1000, sy, (sx2-sx1)/1000, color);
    }
  }
}

void HX8347::drawRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint16_t color) {
  digitalWrite(_cd,HIGH);
  digitalWrite(_cs,LOW);
  // smarter version
  H_line(x+r, y, w-2*r, color);
  H_line(x+r, y+h-1, w-2*r, color);
  V_line(x, y+r, h-2*r, color);
  V_line(x+w-1, y+r, h-2*r, color);
  // draw four corners
  drawCircleHelper(x+r, y+r, r, 1, color);
  drawCircleHelper(x+w-r-1, y+r, r, 2, color);
  drawCircleHelper(x+w-r-1, y+h-r-1, r, 4, color);
  drawCircleHelper(x+r, y+h-r-1, r, 8, color);
  digitalWrite(_cs,HIGH);  
}

void HX8347::goHome(void) {
  goTo(0,0);
}

void HX8347::fillScreen(uint16_t color) {
  //digitalWrite(_cd,HIGH);
  //digitalWrite(_cs,LOW);
  goHome();
  uint32_t i;
  
  i = 320;
  i *= 240;
  
  *portOutputRegister(csport) &= ~cspin;
  //digitalWrite(_cs, LOW);
  *portOutputRegister(cdport) |= cdpin;
  //digitalWrite(_cd, HIGH);
  *portOutputRegister(rdport) |= rdpin;
  //digitalWrite(_rd, HIGH);
  *portOutputRegister(wrport) |= wrpin;
  digitalWrite(_wr, HIGH);

  setWriteDir();
  while (i--) {
    Lcd_Write_Data(color); 
  }

  *portOutputRegister(csport) |= cspin;
  //digitalWrite(_cs, HIGH);
}

/*inline void HX8347::setWriteDir(void) {
#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || defined (__AVR_ATmega328) || (__AVR_ATmega8__)
  DATADDR2 |= DATA2_MASK;
  DATADDR1 |= DATA1_MASK;
#elif defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2561__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280__) 

  #ifdef USE_ADAFRUIT_SHIELD_PINOUT
  DDRH |= 0x78;
  DDRB |= 0xB0;
  DDRG |= _BV(5);
  #else
  MEGA_DATADDR = 0xFF;
  #endif
#else
  #error "No pins defined!"
#endif
}*/

void HX8347::goTo(int x, int y) {
  Address_set(x, x,y,y);     // GRAM Address Set (Horizontal Address) (R20h)
  Lcd_Write_Com(0x0022);            // Write Data to GRAM (R22h)
}

void HX8347::LCD_Clear(unsigned int j)                   
{	
  unsigned int i,m;

  Lcd_Write_Com(0x022); //write_memory_start
  digitalWrite(_cd,HIGH);
  digitalWrite(_cs,LOW);
 // Address_set(0,0,320,240);

  for(i=0;i<320;i++)
    for(m=0;m<240;m++)
    {
      Lcd_Write_Data(j);

    }
  digitalWrite(_cs,HIGH);   
}

HX8347::HX8347(uint8_t cs, uint8_t cd, uint8_t wr, uint8_t rd, uint8_t reset) {
  _cs = cs;
  _cd = cd;
  _wr = wr;
  _rd = rd;
  _reset = reset;
  
  rotation = 0;
  _width = TFTWIDTH;
  _height = TFTHEIGHT;

  // disable the LCD
  digitalWrite(_cs, HIGH);
  pinMode(_cs, OUTPUT);  
  
  digitalWrite(_cd, HIGH);
  pinMode(_cd, OUTPUT);  
  
  digitalWrite(_wr, HIGH);
  pinMode(_wr, OUTPUT);  
  
  digitalWrite(_rd, HIGH);
  pinMode(_rd, OUTPUT);  

  digitalWrite(_reset, HIGH); 
  pinMode(_reset, OUTPUT); 

  /*csport = digitalPinToPort(_cs);
  cdport = digitalPinToPort(_cd);
  wrport = digitalPinToPort(_wr);
  rdport = digitalPinToPort(_rd);

  cspin = digitalPinToBitMask(_cs);
  cdpin = digitalPinToBitMask(_cd);
  wrpin = digitalPinToBitMask(_wr);
  rdpin = digitalPinToBitMask(_rd);
*/
  cursor_y = cursor_x = 0;
  textsize = 1;
  textcolor = 0xFFFF;
}

uint16_t HX8347::width(void) {
  return _width;
}
uint16_t HX8347::height(void) {
  return _height;
}

size_t HX8347::write(uint8_t c) {
  if (c == '\n') {
    cursor_y += textsize*8;
    cursor_x = 0;
  } else if (c == '\r') {
    // skip em
  } else {
    drawChar(cursor_x, cursor_y, c, textcolor, textsize);
    cursor_x += textsize*6;
  }
return 0;
}
