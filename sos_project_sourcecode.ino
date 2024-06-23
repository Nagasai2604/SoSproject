#include <TinyGPS++.h>                                                                                                                                                                                                         //library for gps module
#include <SoftwareSerial.h>               //library for Serial data handler

#define button 13
#define buzzer A4

SoftwareSerial gsm(8,5);          //RX, TX
SoftwareSerial gps_serial(2,3);   //RX, TX

TinyGPSPlus gps;                     //create object for gps device

String number = "8143994218";        //phone number on which you want to send sms

double Lat;       //variable for lattitude
double Lot;       //variable for longitude
double Alt;       //variable for altitude

unsigned long pmillis = 0; 
unsigned long plmillis = 0;
unsigned const bdelay = 100;
int count = 0;


void setup() {
  gsm.begin(9600);                    //assign gsm buadrate
  gps_serial.begin(9600);             //assign gps buadrate

  pinMode(button,INPUT);              //set button pin as input
  pinMode(buzzer,OUTPUT);

  gsm.println("AT+CMGF=1\r");         //gsm text mode
  delay(1000);
  gsm.println("AT+CNMI=2,2,0,0,0\r"); //read new sms mode
  delay(1000);
  gsm.println("AT+CMGD=1,4");         //delete sms
  delay(1000);
}


void loop() {
  if(millis()-plmillis>2000){
    Locationdata();
    plmillis = millis();
  }
  
  if(digitalRead(button)==0){
    if(millis()-pmillis>bdelay){
      count++;
      pmillis = millis();
    }  
  }
  else{
    digitalWrite(buzzer,0);
    count =0;
  }

  if(count == 2){
    call(); 
    delay(1000);
    sms();
    delay(2000);
  }
}


void sms(){
  
  //String msg = "I am in trouble! Lat:"+String(Lat)+",Lot:"+String(Lot)+"Alt:"+String(Alt);
  String msg = "I am in trouble!: http://www.google.com/maps/place/" + String(Lat) + "," + String(Lot);
  delay(100);
  gsm.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(500);
  gsm.println("AT+CMGS=\"+91"+number+"\"\r");    //assign reciver`s number
  delay(1000);
  gsm.println(msg);
  delay(500);
  gsm.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  digitalWrite(buzzer,1);
}


void call(){
  gsm.print("ATD+ ");
  gsm.print("+91"+number);
  gsm.println(";");
  delay(5000);
}


void Locationdata(){
  while (gps_serial.available() > 0){                   //check if serial data  is available
    if (gps.encode(gps_serial.read())){                 //encode Serial data
      if(gps.location.isValid()){                       //if data is valid get location
        Lat = gps.location.lat();                       //update lattitude
        Lot = gps.location.lng();                       //update longiitude
        Alt = gps.altitude.meters();                    //update altitude
      }
      else{                                             //if data is not available
        Serial.println("Please connect gps");
      }
    }
  }
}