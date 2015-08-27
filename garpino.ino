#include <Time.h>
#define TIME_MSG_LEN  11   // time sync to PC is HEADER followed by Unix time_t as ten ASCII digits
#define TIME_HEADER  'T'   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 


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

unsigned long time1 = 1;         

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
  
  setSyncProvider( requestSync);  //set function to call when sync required
  Serial.println("Waiting for sync message");
}

void loop() {
  delay(100);
  Serial.println("Ahoj Svete");

  if(Serial.available() ) {
    processSyncMessage();
  }
  if(timeStatus() == timeNotSet) {
    Serial.println("waiting for sync message");
  }
  else {     
    digitalClockDisplay();  
  }


  //Number of milliseconds since the program started
  time1 = millis();
  //prints time since program started
  Serial.println(time1);


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

void digitalClockDisplay() {
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
}

void printDigits(int digits) {
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void processSyncMessage() {
  // if time sync available from serial port, update time and return true
  while(Serial.available() >=  TIME_MSG_LEN ){  // time message consists of header & 10 ASCII digits
    char c = Serial.read() ; 
    Serial.print(c);  
    if( c == TIME_HEADER ) {       
      time_t pctime = 0;
      for(int i=0; i < TIME_MSG_LEN -1; i++){   
        c = Serial.read();          
        if( c >= '0' && c <= '9'){   
          pctime = (10 * pctime) + (c - '0') ; // convert digits to a number    
        }
      }   
      setTime(pctime);   // Sync Arduino clock to the time received on the serial port
    }  
  }
}

time_t requestSync() {
  Serial.write(TIME_REQUEST);  
  return 0; // the time will be sent later in response to serial mesg
}




