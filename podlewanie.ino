
//sensory analogowe, mierza wilgotnosc
#define HUMIDITY_PIN3  A1
#define HUMIDITY_PIN2  A3
#define HUMIDITY_PIN1  A0
#define HUMIDITY_PIN0  A2

//przyciski, dzialaja na pull down, zmacknuti-high
#define BUTTON2  18
#define BUTTON3  19
#define BUTTON0  6
#define BUTTON1  7

//silniczki
#define MOTOR0  2
#define MOTOR2  3
#define MOTOR3  4
#define MOTOR1  5

#include <LiquidCrystal.h>

LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

void setup() {
  Serial.begin(57600);
  // set up the LCD's number of columns and rows: 
  lcd.begin(20, 4);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON0, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);

  pinMode(MOTOR0, OUTPUT);
  pinMode(MOTOR1, OUTPUT);
  pinMode(MOTOR2, OUTPUT);
  pinMode(MOTOR3, OUTPUT);
}

void loop() {
  delay(100);
  Serial.println("Ahoj Svete");
  if (digitalRead(BUTTON0)==LOW){
    digitalWrite(MOTOR0, HIGH);
  }
  else{
    digitalWrite(MOTOR0, LOW);
  }

  if (digitalRead(BUTTON1)==LOW){
    digitalWrite(MOTOR1, HIGH);
  }
  else{
    digitalWrite(MOTOR1, LOW);
  }
  if (digitalRead(BUTTON2)==LOW){
    digitalWrite(MOTOR2, HIGH);
  }
  else{
    digitalWrite(MOTOR2, LOW);
  }
  if (digitalRead(BUTTON3)==LOW){
    digitalWrite(MOTOR3, HIGH);
  }
  else{
    digitalWrite(MOTOR3, LOW);
  }
  int a0 = analogRead(HUMIDITY_PIN0);
  int a1 = analogRead(HUMIDITY_PIN1);
  int a2 = analogRead(HUMIDITY_PIN2);
  int a3 = analogRead(HUMIDITY_PIN3);
  lcd.setCursor(0, 0);
  lcd.print(a0);
  lcd.setCursor(0, 1);
  lcd.print(a1);
  lcd.setCursor(0, 2);
  lcd.print(a2);
  lcd.setCursor(0, 3);
  lcd.print(a3);
}






