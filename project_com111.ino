#include <SoftwareSerial.h>

#include <Servo.h>

#include <Key.h>
#include <Keypad.h>

#include <LiquidCrystal_I2C.h>

#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <deprecated.h>
#include <require_cpp11.h>
//connection to node mcu
SoftwareSerial espserial(1,0);

#define RST_PIN  9
#define SS_PIN   10
MFRC522 rfid(SS_PIN,RST_PIN);
byte acessUID[4]={0x63, 0xDB, 0x2C, 0x2E};

// Keypad
#define PASSWORD_LENGTH 4
char password[PASSWORD_LENGTH] = {'1', '2', '0', '7'}; // Predefined password

const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns
char keys[ROWS][COLS] = {
  {'1', '2', '8', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {7,6,5,4}; // Connect to the row pinouts of the keypad
byte colPins[COLS] = {3,2,A1,A0}; // Connect to the column pinouts of the keypad//1=a1,0=A0

Keypad keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS); 

LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD address to 0x27 for a 16 chars and 2 line display

char inputPassword[PASSWORD_LENGTH + 1] = {'\0'}; // Store the input password
int passwordIndex = 0; // Keep track of the index of the next character to be entered
int flag = 0;
//servo code
Servo s1;
//Alerting
#define pinMOde
//cloud
int attempt=0;

void setup() {
  // put your setup code here, to run once:
  keypad.setDebounceTime(50);
  SPI.begin();
  rfid.PCD_Init();
  lcd.init();
  lcd.backlight();
  lcd.begin(16,2);
  lcd.clear();
  // display initial message
  lcd.print("Exam Paper");
  lcd.setCursor(0,1);
  lcd.print("Protection");
  delay(2000);
  lcd.clear();
  Serial.begin(9600);
  //servo
  s1.attach(A2); 
  //Alerting
  pinMode(A3,OUTPUT);//buzzor 
  //connecting to the node mcu
  Serial.begin(9600);
  espserial.begin(9600);  
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.begin(16,2);
  lcd.print ("TAG please");
  delay(1000);
  //Serial.println("kdnjn");
  lcd.clear();

  if (!rfid.PICC_IsNewCardPresent()) {
    //Serial.println("1st if");
    return;
  }

  if (!rfid.PICC_ReadCardSerial()) {
    
    return;
  }

  if (rfid.uid.uidByte[0] == acessUID[0] && rfid.uid.uidByte[1] == acessUID[1] && rfid.uid.uidByte[2] == acessUID[2] && rfid.uid.uidByte[3] == acessUID[3]) {
    //Serial.println("entered loop");
    lcd.setCursor(0,0);
    lcd.print("Access Granted");  
    delay(1000);
    lcd.clear();
    flag = 1;
  }

  if (flag == 1) {
    lcd.print("Enter password :");  
    while (true) {
      char key = keypad.getKey();
      //Serial.println(key);
      if (key != NO_KEY) {
        
    if (key == '*' || key == '#') { // Clear the input password
      for (int i = 0; i < PASSWORD_LENGTH; i++) {
        inputPassword[i] = '\0';
      }
      passwordIndex = 0;
      lcd.clear();
      lcd.print("Enter password:");
    } else { // Append the key to the input password
      inputPassword[passwordIndex] = key;
      passwordIndex++;
      lcd.setCursor(passwordIndex - 1, 1);
      lcd.print('*');
      if (passwordIndex == PASSWORD_LENGTH) { // Check the input password
        if (strncmp(inputPassword, password,4) == 0) {
          lcd.clear();
          lcd.print("Password correct");
          Serial.println("password correcct");
          delay(1000);    
          lcd.clear();
          lcd.print("Opening the box");   
          Serial.println("password correcct");
          s1.write(180);
          delay(1000);
          s1.write(90);                                    
          break;
        } 
        else {
          lcd.clear();
          lcd.print("Password incorrect");
          Serial.println("password incorrecct");

          digitalWrite(A3,HIGH);
          delay(1000);
          digitalWrite(A3,LOW);
          delay(1000);
          
                                  
          break;
                  }
        delay(1000);
        lcd.clear();
        for (int i = 0; i < PASSWORD_LENGTH; i++) {
          inputPassword[i] = '\0';
         }
        passwordIndex = 0;
        //lcd.print("Enter password:");
      }
    }
  }
}
  flag=0;
delay(1000);
}

else{
  
lcd.setCursor(0, 0);  
lcd.print("Acess Denied");
Serial.println("wrong rfid kept");
delay(2000);
digitalWrite(A3,HIGH);
delay(1000);
digitalWrite(A3,LOW);
delay(1000);  

}
rfid.PICC_HaltA(); 
}


