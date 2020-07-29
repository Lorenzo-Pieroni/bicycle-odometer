#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SimpleTimer.h>

#define OLED_RESET 4
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

SimpleTimer show;

#define sens 0
#define pinsens 3 //pin del sensore

float difftemp;
float velms;
float velkmh;
int metri = 0;
float metri2;
float km;
float temp1;
float temp2;
int decimals = 0;
int decimalsVel = 1;
int circonferenzaruota = 2; //circonferenza della ruota della bici
int voltage = 4000;
String unit = "m";

void setup() {
  pinMode(pinsens, INPUT_PULLUP);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //Impostare l'indirizzo i2c annotato in precedenza
  display.clearDisplay(); //Pulisce il buffer da inviare al display
  display.setTextSize(3); //Imposta la grandezza del testo
  display.setTextColor(WHITE); //Imposta il colore del testo (Solo bianco)
  display.display();
  show.setInterval(500, showValues);
}

void loop() {
  show.run();
  if (digitalRead(pinsens) == HIGH) {
    metri = metri + circonferenzaruota;
    while (digitalRead(pinsens) == HIGH) {
      temp2 = (millis() / 1000.0);
    }
    if (temp2 > temp1) {
      difftemp = temp2 - temp1;
      velms = circonferenzaruota / difftemp;
      velkmh = velms * 3.6;
      temp1 = millis();
      if (velkmh > 99.9) decimalsVel = 0;
      else decimalsVel = 1;
    }
    if (digitalRead(pinsens) == LOW) {
      while (digitalRead(pinsens) == LOW) {
        temp1 = (millis() / 1000.0);
      }
    }
  }

  if (metri < 1000) {
    metri2 = metri;
    unit = " m";
    decimals = 0;
  }
  else {
    metri2 = metri / 1000.0;
    unit = " Km";
    decimals = 1;
    if (metri2 > 99.9) decimals = 0;
  }
}

void showValues() {
  //clearMod();
  display.clearDisplay();
  battery();
  display.setCursor(0, 6);
  display.print(metri2, decimals);
  display.setCursor(55, 8);
  display.print(unit);
  display.setCursor(0, 38);
  display.print(velkmh, decimalsVel);
  display.setCursor(60, 46);
  display.setTextSize(2); //Imposta la grandezza del testo
  display.println(" km/h");
  display.setTextSize(3); //Imposta la grandezza del testo
  display.display();
}

void clearMod() {
  display.fillRect(0, 0, 64, 60, BLACK);
}

void battery() {
  display.drawRect(96, 1, 26, 9, WHITE); //Disegna la batteria
  display.fillRect(122, 4, 3, 3, WHITE);
  voltage = mapf(analogRead(A1), 0, 1024, 0, 5) * 1000;
  display.fillRect(97, 2, 24, 7, BLACK);
  if (voltage > 3400 && voltage <= 3500) {
    display.fillRect(96, 1, 6, 9, WHITE);
  }

  else if (voltage > 3500 && voltage <= 3600) {
    display.fillRect(96, 1, 13, 9, WHITE);
  }

  else if (voltage > 3600 && voltage <= 3700) {
    display.fillRect(96, 1, 20, 9, WHITE);
  }

  else if (voltage > 3700) {
    display.fillRect(96, 1, 27, 9, WHITE);
  }
}

float mapf(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}
