#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

#define LED_PIN 2
#define RESET_PIN 3


int count = 0;
int slh;

void reset() {
  count = 0;

  digitalWrite(RESET_PIN, HIGH);
  slh = analogRead(2);
  slh = slh * 26 / 1022 + 4; //slh between 4 and 30
  Serial.print(slh, DEC);

  Serial.print("\n");
  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.backlight();
  lcd.print(slh, DEC);
  lcd.print("cm");
  delay(2000);
  lcd.noBacklight();
  lcd.clear();
  digitalWrite(RESET_PIN, LOW);

}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(RESET_PIN, OUTPUT);

  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.backlight();
  lcd.print("Push to start");
  delay(1000);
  reset();
  lcd.noBacklight();
  lcd.clear();
}

void loop() {
  int val;
  int dist ;
  static int old;
  static int j = 0;
  int i;

  for (i = 0; i < 1000; i++) {
    if (analogRead(1) < 10) reset();

    val = analogRead(0);
    if (val < 4) val = 4;
    dist = (6787 / (val - 3)) - 4;

    if (dist < slh ) {
      if (old > slh - 1)   j++;
      else {
        if (j > 5) {
          digitalWrite(LED_PIN, HIGH);
          count++;
          delay(100);
          j = 0;
        }
        else if (j > 0) j++;
      }
    }
    digitalWrite(LED_PIN, LOW);
    old = dist;
  }
  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.print(dist, DEC);
  lcd.print("cm");
  lcd.setCursor(0, 1);
  lcd.print(count, DEC);

}

