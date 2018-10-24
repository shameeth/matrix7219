


#include "matrix7219.h"
// MAX7219 commands:

#define CMD_NOOP   0
#define CMD_DIGIT0 1
#define CMD_DIGIT1 2
#define CMD_DIGIT2 3
#define CMD_DIGIT3 4
#define CMD_DIGIT4 5
#define CMD_DIGIT5 6
#define CMD_DIGIT6 7
#define CMD_DIGIT7 8
#define CMD_DECODEMODE  9
#define CMD_INTENSITY   10
#define CMD_SCANLIMIT   11
#define CMD_SHUTDOWN    12
#define CMD_DISPLAYTEST 15




matrix7219::matrix7219(int din_pin, int clk_pin , int cs_pin, int max_disp)
{
  DIN_PIN = din_pin;
  CLK_PIN = clk_pin;
  CS_PIN  = cs_pin;
  NUM_MAX = max_disp;
  pinMode(DIN_PIN, OUTPUT);
  pinMode(CLK_PIN, OUTPUT);
  pinMode(CS_PIN, OUTPUT); 
 
  
}




void matrix7219::Initmatrix()
{
  digitalWrite(CS_PIN, HIGH);
  sendCmdAll(CMD_DISPLAYTEST, 0);    // dsiaply test OFF
  sendCmdAll(CMD_SCANLIMIT, 7);      //
  sendCmdAll(CMD_DECODEMODE, 0);      // decode mode OFF
  sendCmdAll(CMD_INTENSITY, 0);      
  sendCmdAll(CMD_SHUTDOWN, 1);     // normal Operation mode
  //for(i=0; i<NUM_MAX*8+8; i++){
	  
  //scr[i];}
  clr();
  refreshAll();		
}

void matrix7219::sendCmd(int addr, byte cmd, byte data)
{
  digitalWrite(CS_PIN, LOW);
  for (int i = NUM_MAX-1; i>=0; i--) {
    shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, i==addr ? cmd : 0);
    shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, i==addr ? data : 0);
  }
  digitalWrite(CS_PIN, HIGH);
}

void matrix7219::sendCmdAll(byte cmd, byte data)
{
  digitalWrite(CS_PIN, LOW);
  for (int i = NUM_MAX-1; i>=0; i--) {
    shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, cmd);
    shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, data);
  }
  digitalWrite(CS_PIN, HIGH);
}

void matrix7219::refresh(int addr) {    
  for (int i = 0; i < 8; i++)
    sendCmd(addr, i + CMD_DIGIT0, scr[addr * 8 + i]);
}

void matrix7219::refreshAll() { 
//Serial.println("refresh:");       
  for (int c = 0; c < 8; c++) {
    digitalWrite(CS_PIN, LOW);
    for(int i=NUM_MAX-1; i>=0; i--) {
      shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, CMD_DIGIT0 + c);
      shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, scr[i * 8 + c]);
	 //  Serial.println(i*8+c);
	 //  Serial.print(":");
	 //  Serial.print(scr[i * 8 + c],HEX);
	  // Serial.print(";");
    }
    digitalWrite(CS_PIN, HIGH);
 }
}

void matrix7219::clr()
{
  for (int i = 0; i < NUM_MAX*8; i++) scr[i] = 0;
}

void matrix7219::scrollLeft()
{
  for(int i=0; i < NUM_MAX*8+7; i++) scr[i] = scr[i+1];
}

void matrix7219::invert()
{
  for (int i = 0; i < NUM_MAX*8; i++) scr[i] = ~scr[i];
}


void matrix7219::showDigit(char ch, int col, const uint8_t *data, int dx, int dy)
{
  // Serial.println("store:");
  if(dy<-8 | dy>8) return;
  int len = pgm_read_byte(data);               //  lenth of character bytes per segment
  int w = pgm_read_byte(data + 1 + ch * len);  //  no of coloumn of segment say 4x8 that is 4
  col += dx;
  for (int i = 0; i < w; i++)                   
    if(col+i>=0 && col+i<NUM_MAX*8) {
      byte v = pgm_read_byte(data + 1 + ch * len + 1 + i);   // read the byte from the memory and assign to V	 
      if(!dy) scr[col + i] = v; else scr[col + i] |= dy>0 ? v>>dy : v<<-dy;// store the read byte from Falsh memory into Array scr
	  //Serial.print(scr[col+i],HEX);// no problem in array writing
	  // Serial.print(col+i);
	  // Serial.print(":");
	  // Serial.print(scr[col+i],HEX);
	   //Serial.print(";");
    }
}

int matrix7219::showChar(char ch, int col, const uint8_t *data,int dx, int dy)
{
	//Serial.println(ch,DEC);
	
	//ch -= 32;
  int len = pgm_read_byte(data);
  int w = pgm_read_byte(data + 1 + ch * len);
  if(dy<-8 | dy>8) return w;
  col += dx;                      // x axis shift
  for (int i = 0; i < w; i++)
    if(col+i>=0 && col+i<NUM_MAX*8) {
      byte v = pgm_read_byte(data + 1 + ch * len + 1 + i);
      if(!dy) scr[col + i] = v; else scr[col + i] |= dy>0 ? v>>dy : v<<-dy;   //y axis shift if dy >0
    }
  return w;
}


void matrix7219::setCol(int col, byte v,int dx,int dy)
{
  if(dy<-8 | dy>8) return;
  col += dx;
  if(col>=0 && col<32)
    if(!dy) scr[col] = v; else scr[col] |= dy>0 ? v>>dy : v<<-dy;
}

int matrix7219::printChar(unsigned char ch, const uint8_t *data)
{
  int len = pgm_read_byte(data);
  int i,w = pgm_read_byte(data + 1 + ch * len);
  for (i = 0; i < w; i++)
    scr[NUM_MAX*8 + i] = pgm_read_byte(data + 1 + ch * len + 1 + i);
  scr[NUM_MAX*8 + i] = 0;
  return w;
}

/*void matrix7219::scrollChar(unsigned char c, int del) {
  c = convertPolish(c);
  if (c < ' ' || c > '~'+22) return;
  c -= 32;
  int w = printChar(c, font);
  for (int i=0; i<w+1; i++) {
    delay(del);
    scrollLeft();
    refreshAll();
  }
}
void matrix7219::scrollString(char *s, int del)
{
  while(*s) scrollChar(*s++, del);
}*/




