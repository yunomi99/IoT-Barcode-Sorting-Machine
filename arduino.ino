//PEMANGGILAN LIBRARY YANG DIGUNAKAN PADA PROGRAM ALAT
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial mySerial(3, 2); // RX, TX
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo servo0;
Servo servo1;

#define none 0
#define box1 115
#define box2 116
#define box3 117
#define berjalan 118
#define selesai 119

const int IR1 = 4;
const int IR2 = 5;
const int buttonStart = 9; 
const int pinMotor = A0;
bool dataIR1, dataIR2;
int statePisah = none;
char dataBarcode = ' ';

void setup() {
  //INISIALISASI LCD
  lcd.begin();
  Serial.begin(9600);
  mySerial.begin(9600);

  //MENGHIDUPKAN LAMPU LATAR LCD
  lcd.backlight();

  //MENAMPILKAN NAMA DAN NIM SELAMA 2 DETIK
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("GUSDINI VIVIANI");
  lcd.setCursor(0,1); 
  lcd.print(" 15130080/2015 ");
  delay(1500);

  //INISIALISASI SERVO ATTACH DENGAN PIN YANG DIPAKAI PADA ARDUINO UNO
  servo0.attach(11);
  servo1.attach(10);

  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  
  pinMode(pinMotor, OUTPUT);
  pinMode(buttonStart, INPUT_PULLUP);

  //SET MOTOR MATI DIAWAL
  digitalWrite(pinMotor, LOW);
  
  //NORMALISASI POSISI SERVO
  servo0.write(0);
  servo1.write(0);
  
  //MEMBACA TOMBOL START
  if(digitalRead(buttonStart) == LOW){
    while(digitalRead(buttonStart) == LOW){} 
    }
}

void loop() {  
  //MEMBACA SENSOR INFRARED
  dataIR1 = digitalRead(IR1);
  dataIR2 = digitalRead(IR2);
  
  //BACA SENSOR BARCODE
  if(dataBarcode == ' '){
    if(mySerial.available()) {
      int intBarcode = mySerial.read();
      dataBarcode = (char)intBarcode;
      if(dataBarcode == 'A' || dataBarcode == 'B') {Serial.write(intBarcode);}
    }
  }
  
  if(dataBarcode == 'A'){digitalWrite(pinMotor, HIGH); statePisah = box1;}
  else if(dataBarcode == 'B'){digitalWrite(pinMotor, HIGH); statePisah = box2;}
  else if ((int)dataBarcode == 13 || dataBarcode == ' '){digitalWrite(pinMotor, LOW); dataBarcode = ' ';}
  else if((int)dataBarcode != 13 || dataBarcode != ' '){digitalWrite(pinMotor, HIGH); statePisah = box3;}
  
  //DISPLAY DATA LCD
  lcd.setCursor(0,0);
  lcd.print("STATUS: RUNNING");
  lcd.setCursor(0,1);
  if(dataBarcode == 'A' || dataBarcode == 'B'){lcd.print("Kota Tujuan = " + (String)dataBarcode);}
  else{lcd.print("Kota Tujuan = ?");}
  

  //MEMILIH BOX BERDASARKAN SENSOR
  if(statePisah == box1){keBox1();}
  else if(statePisah == box2){keBox2();}
  else if(statePisah == box3){keBox3();}
}

void keBox1(){
  if(dataIR1 == 0){
    servo0.write(45);
    servo1.write(0); 
    delay(3000);
    servo0.write(0); 
    servo1.write(0);
    digitalWrite(pinMotor, LOW);
    dataBarcode = ' ';
    statePisah = none;
    mySerialFlush();
    }  
  }
  
void keBox2(){
  if(dataIR2 == 0){
    servo0.write(0); 
    servo1.write(50); 
    delay(3000);
    servo0.write(0); 
    servo1.write(0);
    digitalWrite(pinMotor, LOW);
    dataBarcode = ' ';
    statePisah = none;
    mySerialFlush();
    }
  }

void keBox3(){
  if(dataIR2 == 0){
    delay(3000);
    digitalWrite(pinMotor, LOW);
    dataBarcode = ' ';
    statePisah = none;
    mySerialFlush();
    }
  }

//MEMBERSIHKAN BUFFER SOFTWARE SERIAL
void mySerialFlush(){
  while(mySerial.available() > 0) {
    char t = mySerial.read();
  }
}   
