#include <LiquidCrystal.h> 
#include <virtuabotixRTC.h>        
#include "HX711.h"
#include <time.h>
#define calibration_factor -7050.0 
#define DOUT  6
#define CLK  7

LiquidCrystal lcd(12,11,5,4,3,14);
virtuabotixRTC myRTC(8, 9, 10);
HX711 scale(DOUT, CLK);
int rtime = 0;
int showWindow = 0;
int btn = 18, usi = 13, uso = 2, buz = 15;
int hoursControl = 0, minControl = 0;
int num=0, num1 = 0;
int nTen=0,nFifty=0,nHundred=0,nFiveHundred=0, total=0;
int cnt=0,cnt1=0,cnt2=0,cnt3=0;
float prevWeight=0.0,weight=0.0;


bool ctrl=false;
void setup()
{
  pinMode(uso, OUTPUT);
  pinMode(usi, INPUT);
  pinMode(buz, OUTPUT);
  pinMode(btn, INPUT);
  lcd.setCursor(0, 0);
  lcd.begin(20, 4); 
  scale.set_scale(calibration_factor); 
  scale.tare();
  Serial.begin(9600);
  
   srand((unsigned)time(NULL));
  /*myRTC.setDS1302Time(00, 45, 14, 0, 5, 12, 2016);*/
}

/* Main program */
void loop()
{
  int randNum1=rand()%2+1;
  int randNum2=rand()%2+1;
  
  myRTC.updateTime();
  weight = scale.get_units();

 
  if(weight< 0) weight=0;
   // Serial.println(weight);
  
  if(weight >=1.4 &&weight <=2){
    cnt++;
    if(cnt==10){
    Serial.println("500won");
    scale.tare();
    nFiveHundred++;
    cnt=0;
    }
  }
  else if(weight < 1.4 && weight >= 0.9){
    cnt1++;
    if(cnt1==10){
      Serial.println("100won");
    nHundred++;
   scale.tare();
   cnt1=0;
    }
  }
else if(weight < 0.8 && weight >=0.4){
    cnt2++;
    if(cnt2==10){
      Serial.println("50won");
    nFifty++;
   scale.tare();
   cnt2=0;
    }
  }
  else if(weight < 0.4 && weight >=0.2){
    cnt3++;
    if(cnt3==10){
      Serial.println("10won");
    nTen++;
   scale.tare();
   cnt3=0;
    }
  }
  if(hoursControl == myRTC.hours && minControl == myRTC.minutes && 0 == myRTC.seconds && showWindow != 1 ){
 
      while(1) {
      lcd.clear();
      lcd.print("It's ");
      lcd.print(hoursControl);
      lcd.print(" : ");
      lcd.print(minControl);
      tone(buz, 440);
      digitalWrite(uso, HIGH);
      delayMicroseconds(20);
      digitalWrite(uso, LOW);
      delayMicroseconds(20);
      int duration = pulseIn(usi, HIGH);
      int centimeter = duration/58.2;
      
 
      lcd.print("           1");lcd.print(" X ");lcd.print(randNum2);lcd.print("= ? 100won ");lcd.print(num);
      lcd.print("  Insert ");
      if(centimeter <= 4 && centimeter >= 1) {//초음파 잡히고
          num++;

        if(num==randNum2)
        {   
        noTone(buz);
        nHundred+=num;
        num=0;
        delay(200);
        scale.tare();
     
        break;
        }
        delay(20);
      }//알람 키고 끄는거
      delay(25);
    }
  }
  if(digitalRead(btn) == HIGH) {
    showWindow++;
    if(showWindow > 3) showWindow = 0;
  }
  if(showWindow == 0) {//시계
    lcd.clear();
    lcd.print("     ");
    lcd.print(myRTC.year);
    lcd.print("/");
    lcd.print(myRTC.month);
    lcd.print("/");
    lcd.print(myRTC.dayofmonth);
    lcd.print("     ");
    lcd.print("       ");
    lcd.print(myRTC.hours);
    lcd.print(":");
    lcd.print(myRTC.minutes);
    lcd.print(":");
    lcd.print(myRTC.seconds);
    lcd.print("         ");
    if(myRTC.seconds < 10)
    lcd.print(" ");
    }
    else if(showWindow==1) {//알람 세팅
        hoursControl = analogRead(A2);
        minControl = analogRead(A3);
        hoursControl = map(hoursControl, 0, 1023, 0, 24);
        minControl = map(minControl, 0, 1023, 0, 60);
        lcd.clear();
        lcd.print("     Set Alaram           ");
        lcd.print(hoursControl);
        lcd.print(" : ");
        lcd.print(minControl);
    }
    else if(showWindow==2){//동전 카운트 
     lcd.clear();
     if(nTen > 10) {
     lcd.print("10won :");lcd.print(nTen); lcd.print(" ");
     } else { 
      lcd.print("10won :");lcd.print(nTen); lcd.print("  ");
     }
     if(nFifty > 10) {
     lcd.print("50won :");lcd.print(nFifty); lcd.print(" ");
     } else {
      lcd.print("50won :");lcd.print(nFifty); lcd.print("  ");
     }
     lcd.print("100won:");lcd.print(nHundred); lcd.print("  ");
     lcd.print("500won:");lcd.print(nFiveHundred); 
     lcd.print("  ");
    }
    else if (showWindow=3){//총합
     lcd.clear();
     total=100*nHundred+500*nFiveHundred+50*nFifty+10*nTen;
     lcd.print("Total : "); lcd.print(total);
    }
  
    delay(50);
}
