#include <TFT_ST7735.h> // Graphics and font library for ILI9341 driver chip
#include <SPI.h>
#include <Wire.h>//RTCLib
#include "bitmaps.h"
#include <DS3231.h>
TFT_ST7735 tft = TFT_ST7735();  // Invoke library, pins defined in User_Setup.h

unsigned long drawTime = 20;
byte red = 31;
byte green = 0;
byte blue = 0;
byte state = 0;
unsigned int colour = red << 11; // Colour order is RGB 5+6+5 bits each
unsigned long targetTime = 0;
DS3231 Clock;

//RawData
int motorWaterTemp = 50;
int tempExt = -1;
int fuelTank = 10;

//DisplayString
String  strmotorWaterTemp = "- C";
String  strtempExt = "- C";
String  strfuelTank = "00L";

bool h12;
bool PM;
int second, minute, hour, date, month, year, temperature;

char  PrevstrmotorWaterTemp = "- - C";
char  PrevstrtempExt = "- - C";
char  PrevstrfuelTank = "- - C";
char  PrevstrTime = "-";

#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

//DisplayMode
// 0 RPM
// 1 KM/H
int ScreenDisplay = 0;
int timerDP = 0;


char ftime[12];
char fueldTank[6];
char extTemp[8];
char varlRPM[8];


void setup(void) {

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Wire.begin();
  tft.init();

  tft.setRotation(0);//VERTICAL
  //tft.fillScreen(TFT_WHITE);
  tft.drawBitmap(0, 0, splash , 128, 160, ST7735_WHITE);

  delay(550);
  tft.setTextColor(TFT_BLACK);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  tft.drawCentreString("Chargement.", 60, 100, 2);
  delay(750);
  digitalWrite(2, LOW);
  delay(450);
  tft.fillScreen(TFT_BLACK);
  digitalWrite(2, HIGH);
  targetTime = millis() + 1000;
  pinMode(2, INPUT_PULLUP);

  tft.drawFastHLine(0, 14, tft.width(), colour);
  tft.drawFastHLine(0, 15, tft.width(), colour);

  tft.drawFastHLine(0, 135, tft.width(), colour);
  tft.drawFastHLine(0, 136, tft.width(), colour);

  //  strTime = Clock.getMinute();
  // dtostrf(Clock.getTemperature(), 5, 0, strtempExt);

//  varlRPM = "arduino"';
    String flevel = String("9999");
  flevel.toCharArray(varlRPM, 5);
}


void loop() {
  second = Clock.getSecond();
  minute = Clock.getMinute();
  hour = Clock.getHour(h12, PM);
  String st =  String(hour, DEC) + ":" + String(minute, DEC) + ":" + String(second, DEC);
  st.toCharArray(ftime, 12);

  String ft =  String("00L");
  ft.toCharArray(fueldTank, 6);

  String extM =  String(Clock.getTemperature()) + "C";
  extM.toCharArray(extTemp, 8);

  

  //Draw Temperature Moteur Eau
  tft.setTextColor(WHITE, BLACK);


  tft.setCursor(2, 2);
  tft.print(String(strmotorWaterTemp));

  //Draw hour:minute
  tft.drawCentreString(ftime, 64, 2, 1);
  //Draw Fual tank
  tft.drawRightString(fueldTank, 128, 2, 1);

  //Draw Fual tank
  tft.setTextColor(WHITE, BLACK);
  tft.drawRightString(extTemp, 128, 140, 1);
  tft.drawRightString("- C", 128, 150, 1);

  tft.drawRightString("0 KM", 0, 140, 1);
  tft.drawRightString("0 Min", 0, 150, 1);


  timerDP++;

  if(timerDP > 1800){
    if(ScreenDisplay == 0){
      ScreenDisplay = 1;
    }else if(ScreenDisplay == 1){
      ScreenDisplay = 0;
    }
    timerDP = 0;
  }
  //TOBLACK

  displayMode();

  //Draw Temperature Moteur Eau
  //Draw hour:minute
  tft.drawCentreString(ftime, 64, 2, 1);
  //Draw Fual tank
  tft.drawRightString(fueldTank, 128, 2, 1);

  //Draw Fual tank
  tft.setTextColor(WHITE, BLACK);
  tft.drawRightString(extTemp, 128, 140, 1);
  tft.drawRightString("- C", 128, 150, 1);

  tft.drawRightString("0 KM", 0, 140, 1);
  tft.drawRightString("0 Min", 0, 150, 1);

  delay(1500);
}


void displayMode() {
  switch (ScreenDisplay) {
    case 1:
      displayKMH();
      break;
    case 2:
      //do something when var equals 2
      break;
    default:
      displayRPM();
      break;
  }
}

void displayRPM() {

    String flevel = String(analogRead(0));
  flevel.toCharArray(varlRPM, 5);

  tft.setTextColor(WHITE, BLACK);
  tft.drawCentreString(varlRPM, 64, 40, 7);
  tft.drawCentreString("RPM", 64, 95, 4);
    
  tft.setTextColor(WHITE, BLACK);
  tft.drawCentreString(varlRPM, 64, 40, 7);
  tft.drawCentreString("RPM", 64, 95, 4);
  

}


void displayKMH() {
  tft.setTextColor(WHITE, BLACK);
  tft.drawCentreString("0", 64, 40, 7);
  tft.drawCentreString("KM/H", 64, 95, 4);

  tft.setTextColor(WHITE, BLACK);
  tft.drawCentreString("0", 64, 40, 7);
  tft.drawCentreString("KM/H", 64, 95, 4);
}
