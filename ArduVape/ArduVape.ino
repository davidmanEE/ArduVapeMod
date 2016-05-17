#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);


int coilFireGate = 9;   //Pin controlling NMOS pulsing coil
int posEnable = 7;      //Pin controlling PMOS supplying coil
int resCoil = A0;       //Pin reading resistance of coil
int resCheckGate = 8;   //Pin controlling 5V into coil for resistance check
int buttonPin = 2;      //Pin reading button, interrupt
int enabled = 1;        //Var allowing for coil to be energized
int clickCounter = 0;   //Counter to allow for locking/unlocking
int powerLevel = 0;     //Wattage setting for coil
int resetCounter = 0;   //Counter to reset clickCounter after 3 seconds

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.clearDisplay();
    // draw the first ~12 characters in the font
  testdrawchar();
  display.display();
  
  //attachInterrupt(digitalPinToInterrupt(buttonPin), buttonInterrupt, RISING);   //set button pin as interrupt when held high
  pinMode(coilFireGate, OUTPUT);                                              //Configure pin direction
  pinMode(posEnable, OUTPUT);
  pinMode(resCoil, INPUT);
  pinMode(buttonPin, INPUT);

  Serial.begin(9600);                                                         //Enable serial for debug
}

/*void buttonInterrupt() {
  clickCounter++;
  Serial.println("buttonPin HIGH");
  if (clickCounter >= 5)    //Go into lock sequence when
  {
    lock();
  }

  if (enabled == 1) {
    Serial.println("Interrupt...");
    while (digitalRead(buttonPin) == HIGH)
    {
      analogWrite(powerLevel, HIGH);
      delay(10);
    }
  }
  delay(1000);
  }*/

void loop() {
  analogWrite(coilFireGate, 50);
  digitalWrite(posEnable, HIGH);
  if (digitalRead(buttonPin) == HIGH)
  {
    buttonClicked();
  }
  //double resistance = resistanceCheck();
  //Serial.println(resistance);
  //Serial.println("Average Resistance");
  //Serial.print(resistance);

  //delay(1000);
  if (resetCounter >= 30) {    //Reset click counter to prevent unneeded lock
    clickCounter = 0;
    resetCounter = 0;
  }
  resetCounter++;
  delay(100);

}

void buttonClicked() {
  clickCounter++;   //Increment clickCounter
  Serial.println("buttonPin HIGH");
  Serial.println(clickCounter);
  while (digitalRead(buttonPin) == HIGH) {
    if (clickCounter >= 5)    //Go into lock sequence when
    {
      clickCounter = 0;
      if(enabled == 1){
      lock();
      }
      else if (enabled == 0){
        enabled = 1;
      }
    }
    if (enabled == 1)
    {
      fire();
    }
  }
}

void fire() {
  while (digitalRead(buttonPin) == HIGH) {

    Serial.println("Firing!");
    while (digitalRead(buttonPin) == HIGH)
    {
      analogWrite(powerLevel, HIGH);
      delay(10);
    }

  }
}

double resistanceCheck()
{
  int i = 0;
  double resistance = 0;
  double rawADC;
  digitalWrite(resCheckGate, HIGH);   //Enable Resistance Check MOSFET
  for (i = 0; i < 10; i++)            //Take 10 Resistance Samples
  {
    rawADC = analogRead(resCoil);
    resistance += (10 - (rawADC / 1024) * 10);
    Serial.println(resistance);
    delay(10);
  }
  digitalWrite(resCheckGate, LOW);    //Disable Resistance Check MOSFET
  double average = resistance / 10;   //Divide sum of 10 samples by 10
  return (average);
}

void lock() {
  enabled = 0;
  Serial.println("Locked");

}

void mainScreen()
{
  
}


