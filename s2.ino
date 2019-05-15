/* This code is to use with Adafruit FPM10A or DY50 Fingerprint optical Sensor and lcd i2c, it's a modification from the Adafruit example
 * Wiring Sensor/Uno  GND/GND 3.3v/3.3v RX/D3 TX/D2 || LCD i2c/UNO GND/GND Vcc/5v SDA/A4 SCL/A5
 * The code scans the finger when you place it on the sensor and shows you the name and Id if found a match
 * Refer To SurtrTech for more information || Lebouiha A.Y
 */

#include <Adafruit_Fingerprint.h>    //Libraries needed
#include <SoftwareSerial.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define I2C_ADDR 0x27          //LCD i2c stuff
#define BACKLIGHT_PIN 3
#define En_pin 2
#define Rw_pin 1
#define Rs_pin 0
#define D4_pin 4
#define D5_pin 5
#define D6_pin 6
#define D7_pin 7

String Names[] = { "soufa", "binco", "ripou", "azerag" }; //This is the array of names we declare you can add if you want
                                                               //Don't forget the order so Yassine has ID 1 but his place is Names[0]

SoftwareSerial mySerial(2, 3);                                 //Connects with TX with D2 and RX with D3
LiquidCrystal_I2C lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin); //LCD declaring

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);                    //Fingerprin sensor declaring

void setup()  
{
  Serial.begin(9600);
  finger.begin(57600);              //Sensor baude rate
  lcd.begin (16,2);
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home ();
  finger.getTemplateCount();        //Counts the number of templates stored in the sensor flash memory
  lcd.setCursor(0,0);
  lcd.print("Scan ready");
}

void loop()                     
{
  getFingerprintIDez(); //The only function we are running :D
  delay(50);            //don't need to run this at full speed.
}


// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();        //Image scanning
  if (p != FINGERPRINT_OK)  return -1;  

  p = finger.image2Tz();               //Converting
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();     //Looking for matches in the internal memory
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  lcd.clear();
  lcd.print("Found match !");
  lcd.setCursor(0,1);
  lcd.print(finger.fingerID);           //If a match is found it shows the ID of the finger
  lcd.setCursor(2,1);
  lcd.print(Names[finger.fingerID-1]); //Then the name we gave it to it and the -1 is to remove the shift
  delay(3000);
  lcd.clear();
  lcd.print("Scan ready");
  return finger.fingerID; 
  
}
