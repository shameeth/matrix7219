/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/
#ifndef matrix7219_h
#define matrix7219_h

#include "Arduino.h"

class matrix7219
{
  public:
    matrix7219(int DIN_PIN, int CLK_PIN, int CS_PIN int NUM_MAX);
    void sendCmd(int addr, byte cmd, byte data);
	void sendCmdAll(byte cmd, byte data);
	void refresh(int addr);
	void refreshAll();
	void clr();
	void scrollLeft();
	void invert();
	void showDigit(char ch, int col, const uint8_t *data,int dx, int dy)
	void showChar(char ch, int col, const uint8_t *data);
	void setCol(int col, byte v);
	void printChar(unsigned char ch, const uint8_t *data);
	void scrollChar(unsigned char c, int del);
	void scrollString(char *s, int del);
  private:
   
};

#endif