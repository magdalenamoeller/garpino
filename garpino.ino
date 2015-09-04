#include <Time.h>
#define TIME_MSG_LEN  11   // time sync to PC is HEADER followed by Unix time_t as ten ASCII digits
#define TIME_HEADER  'T'   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 
#define COMMAND_HEADER 'C'
#define COMMAND_START "START"
#define COMMAND_STOP "STOP"

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

const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013

LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

time_t measurement_time = 0;
time_t start_time = 0;

#define HOURS_8 8//28800000 //default interval for measurements
#define HUMIDITY_THRESHOLD 300
static uint32_t measurmentInterval = 120;//28800;  in seconds!
static uint32_t waterInterval = 20; //in seconds

int a0, a1, a2, a3; //analog input humidity values

boolean isPumpingWater = false;

#define serialBufferSize 50
char inputBuffer[serialBufferSize];
int serialIndex = 0;


void setup() {
  setTime(DEFAULT_TIME);
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

  //setSyncProvider( requestSync);  //set function to call when sync required
  //Serial.println("Waiting for sync message");

}

void loop() {
  delay(100);

  if(checkSerial()) processCommand(inputBuffer); 

  if( isPumpingWater == false ) {
    serveTheButtons();
  }

  lcd.clear();
  time_t now_t = now();
  digitalClockLCD(now_t, 0);

  if(isTimeToMeasure()) {
    a0 = analogRead(HUMIDITY_PIN0);
    a1 = analogRead(HUMIDITY_PIN1);
    a2 = analogRead(HUMIDITY_PIN2);
    a3 = analogRead(HUMIDITY_PIN3); 


    if(isTooDry(a0)) {
      motorStart(); 
    }
  }

  showLastMeasurement(a0,a1,a2,a3);
  digitalClockLCD(start_time, 2);

  if (isPumpingWater && isTimeToStopMotor()) {
    motorStop();  
  }

}//end loop


void motorStart() {
  start_time = now();
  digitalWrite(MOTOR0, HIGH); //put the water there
  isPumpingWater = true;
}

void motorStop() {
  digitalWrite(MOTOR0, LOW);
  isPumpingWater = false;
}


boolean isTooDry(int aHumidity) {
  if (  aHumidity < HUMIDITY_THRESHOLD ) {
    return true;
  } 
  else {
    return false;
  }
}

boolean isTimeToStopMotor() {
  //lcd.setCursor(0,2);  lcd.print(measurement_time + measurmentInterval);
  //lcd.setCursor(0,3);  lcd.print(time_now);
  time_t time_now = now(); 

  if( time_now >  (start_time + waterInterval) ) { //all in seconds
    return true;
  }
  return false;
}


void digitalClockLCD(time_t t, int row) {
  printLCDDigits(hour(t), 0, row);
  lcd.setCursor(2,row); 
  lcd.print(":");
  printLCDDigits(minute(t), 3, row);
  lcd.setCursor(5, row);
  lcd.print(":");
  printLCDDigits(second(t), 6, row);
}

void showLastMeasurement(int a0, int a1, int a2,int a3) {
  lcd.setCursor(0,1);
  lcd.print("HUM:"); 

  lcd.setCursor(4,1);
  lcd.print(a0); 
  lcd.setCursor(8,1);
  lcd.print(a1); 
  lcd.setCursor(12,1);
  lcd.print(a2); 
  lcd.setCursor(16,1);
  lcd.print(a3); 

}


boolean isTimeToMeasure() {
  time_t time_now = now(); 

  if( time_now >  (measurement_time + measurmentInterval) ) { //all in seconds
    measurement_time = time_now;
    return true;
  }
  return false;
}

void digitalClockDisplay(){
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

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void printLCDDigits(int value, int column, int row) {
  if(value < 10 ) {
    lcd.setCursor(column, row); 
    lcd.print("0");
    lcd.setCursor(column + 1, row); 
    lcd.print(value);  
  } 
  else {
    lcd.setCursor(column, row); 
    lcd.print(value); 
  }
}

time_t requestSync()
{
  Serial.write(TIME_REQUEST);  
  return 0; // the time will be sent later in response to serial mesg
}

void serveTheButtons() {
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
}

void processCommand(char * commandBuffer) {
  unsigned long pctime;
  String command;

  if(timeStatus() == timeNotSet || now() < DEFAULT_TIME ) {
    Serial.println("waiting for sync message");
  }
  //else {     
    //digitalClockDisplay();  
  //}

  //Serial.println(commandBuffer);

  if( commandBuffer[0] == TIME_HEADER) {
    commandBuffer++;
    pctime = strtoul(commandBuffer,NULL,10);
    if( pctime >= DEFAULT_TIME) { // check the integer is a valid time (greater than Jan 1 2013)
      //setTime(pctime); // Sync Arduino clock to the time received on the serial port
      Serial.println("Time set");
    } else {
      Serial.println("waiting for sync message");
    }
  } 
  else if ( commandBuffer[0] == COMMAND_HEADER) {
    commandBuffer++;
    if ( strcmp(commandBuffer, COMMAND_START) == 0 ) {
      motorStart();
      Serial.println("Motor STARTed");
    } 
    else if( strcmp(commandBuffer, COMMAND_STOP) == 0 ) {
      motorStop();
      Serial.println("Motor STOPed");
    }
    else if(strcmp(commandBuffer, "GIVEHUM") == 0) {
      Serial.print(now()); 
      Serial.print(';'); 
      Serial.print(a0); 
      Serial.print(';'); 
      Serial.print(a1); 
      Serial.print(';');
      Serial.print(a2); 
      Serial.print(';');
      Serial.print(a3); 
      Serial.print('\n');
    }
  }
  else {
    Serial.print("Unknown command: "); 
    Serial.println(commandBuffer);
  }
}

boolean checkSerial() {
  boolean isLineFound = false;
  while(Serial.available() > 0 ) {
    char charBuffer = Serial.read();
    if(charBuffer == '\n') {
      inputBuffer[serialIndex] = 0; //terminate the string
      isLineFound = (serialIndex > 0 );
      serialIndex = 0;
    }
    else if(charBuffer == '\r') {
      //ignore it?
    }
    else if ( serialIndex < serialBufferSize && isLineFound == false) {
      inputBuffer[serialIndex++] = charBuffer;
    }

  } 
  return isLineFound;
}















