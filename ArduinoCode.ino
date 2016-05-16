int coilFireGate = 9;
int posEnable = 7;
int resCoil = A0;
int resCheckGate = 8;
int buttonPin = 2;
int enabled = 0;
int clickCounter = 0;
int powerLevel = 0;
int resetCounter = 0;

void setup() {
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonInterrupt, HIGH);
  pinMode(coilFireGate, OUTPUT);
  pinMode(posEnable, OUTPUT);
  pinMode(resCoil, INPUT);
  pinMode(buttonPin, INPUT);

  Serial.begin(9600);
}

void buttonInterrupt() {
  clickCounter++;

  if (clickCounter >= 5)    //Go into lock sequence when
  {
    lock();
  }

  if (enabled == 1) {
    while (digitalRead(buttonPin) == HIGH)
    {
      Serial.println("Interrupt...");
      analogWrite(powerLevel, HIGH);
      delay(10);
    }
  }
  delay(100);
}

void loop() {
  analogWrite(coilFireGate, 50);
  digitalWrite(posEnable, HIGH);

  //double resistance = resistanceCheck();
  //Serial.println(resistance);
  //Serial.println("Average Resistance");
  //Serial.print(resistance);

  //delay(1000);
  if (resetCounter >= 6) {    //Reset click counter to prevent unneeded lock
    clickCounter = 0;
  }
  resetCounter++;
  delay(500);
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
  if (clickCounter >= 5) {
    clickCounter = 0;
    enabled = 1;
  }
  enabled = 0;

}

void display() {


}
