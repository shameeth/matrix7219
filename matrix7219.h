
#ifndef matrix7219_h
#define matrix7219_h

#include <Arduino.h>



class matrix7219
{
  public:
	
    matrix7219(int din_pin, int clk_pin, int cs_pin,int max_disp);
	void Initmatrix();
    void sendCmd(int addr, byte cmd, byte data);
	void sendCmdAll(byte cmd, byte data);
	void refresh(int addr);
	void refreshAll();
	void clr();
	void scrollLeft();
	void invert();
	void showDigit(char ch, int col, const uint8_t *data,int dx, int dy);
	int showChar(char ch, int col, const uint8_t *data, int dx, int dy);
	void setCol(int col, byte v,int dx,int dy);
	int  printChar(unsigned char ch, const uint8_t *data);
	byte scr[40];
	 //int i;
	//byte scr[NUM_MAX*8+8]; // +8 for scrolled char 

	//void scrollChar(unsigned char c, int del);
	//void scrollString(char *s, int del);
	
  private:
	int NUM_MAX ;
	int DIN_PIN; 
	int CLK_PIN; 
	int CS_PIN ;

   
};

#endif
