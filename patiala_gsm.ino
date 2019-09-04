//Mobile Phone controlled industrial Switch By Meharban Singh(OptimusLabs.in)

#include <RTClib.h>
#include <TimeLib.h>
#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(9, 8, 10, 11, 12, 13);
SoftwareSerial mehar(2,3);
RTC_DS1307 rtc;

byte flag0=0;
byte flag1=0;
byte equip[2]={5,6};
byte toggle=4;
byte mode=1;
byte first_time=0;
byte present_state=0;

void configureModem()
{
  Serial.println("Setting Mode");
  mehar.println("AT + CMGF=1\r");
  delay(1000);

  mehar.println("AT+CNMI=2,2,0,0,0\r");
  delay(1000);
  for(int i=0;i<50;i++)
  {
    //Serial.println("in loop 2");
    if(mehar.available())
    {
      Serial.write(mehar.read());
    }
    delay(1);
  }

}

void sendMessage(String msg)
{
  Serial.println("in send message");
  mehar.println("AT + CMGS=\"+919914297270\"");
  delay(300);
  mehar.println(msg);
  delay(3000);
  mehar.println((char)26);
  delay(3000);
  Serial.println("Exit send sms");
}

void deleteMessage()
{
  delay(2000);
  mehar.println("AT + CMGD=1");
  delay(2000);
  mehar.println("AT + CMGD=2");
  delay(2000);
  mehar.println("AT + CMGD=3");
  delay(2000);
  mehar.println("AT + CMGD=4");
  delay(2000);
  mehar.println("AT + CMGD=5");
}

void init_display()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Optimus Welcomes");
  delay(100);
  lcd.setCursor(6,1);
  lcd.print("You");
  delay(100);
}

void status_display(int m)
{
  lcd.clear();
  lcd.setCursor(0,0);
  if(m==0){lcd.print("Manual Mode");}if(m==1){lcd.print("Timer Mode "); lcd.print(toggle);}
  delay(100);
  lcd.setCursor(0,1);
  lcd.print("AC1:"); if(flag0==0){lcd.print("OFF");} if(flag0==1){lcd.print("ON");}
  delay(100);  
  lcd.print("  AC2:");if(flag1==0){lcd.print("OFF");} if(flag1==1){lcd.print("ON");}
  delay(100);
}

void rcv_msg()
{
  lcd.clear();
  lcd.setCursor(5,0);
  lcd.print("Message");
  lcd.setCursor(5,1);
  lcd.print("Received");
  delay(1500);
}

void setup()
{
  lcd.begin(16, 2);
  init_display();
  Wire.begin();
  rtc.begin();
  DateTime now = rtc.now();
  setSyncProvider(RTC.get);   // the function to get the time from the RTC
  while(! rtc.isrunning()){Serial.println("RTC is NOT running!");delay(1000);}
  for(int i=0;i<2;i++){pinMode(equip[i],OUTPUT);digitalWrite(equip[i],LOW);}
  toggle=EEPROM.read(0);mode=EEPROM.read(1);flag0=EEPROM.read(2);flag1=EEPROM.read(3);
  if(mode==0){digitalWrite(equip[0],flag0);delay(2000);digitalWrite(equip[1],flag1);delay(2000);}
  
  Serial.begin(9600);
  mehar.begin(9600);
  Serial.println("Program Started");
  
  delay(20000);
  configureModem();
  deleteMessage();
  delay(1000);
  status_display(mode);
  //sendMessage("automatic");
}


void mode_set(int z)
{
  EEPROM.write(1,z);
  flag0=0;
  flag1=0;
  EEPROM.write(2,flag0);
  EEPROM.write(3,flag1);
  mode=z;
  if(mode==0)
  {
    delay(100);
    digitalWrite(equip[0],flag0);
    delay(2000);
    digitalWrite(equip[1],flag1);
    delay(5000);
  }
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



void loop()
{
  //digitalClockDisplay();delay(2000);
  /*
  delay(1);
  count++;
  if(count>=3100){count=0;}
  
  Serial.println("Inloop");
  Serial.println(mode);  
  Serial.println(count);
  
  if((mode==1) && (count>500))
  {
    count=0;
    Serial.println("In timer");
  int st;int h;tmElements_t tm;
  
  if (RTC.read(tm)) {h=tm.Minute;}//{h=tm.Hour;}//
  Serial.println(h);
  delay(2000);
  /*if (toggle == 4) {if ((h >= 0 && h <= 3) || (h >= 8 && h <= 11) || (h >= 16 && h <= 19)) { st = 1;} else { st = 0; }}
  if (toggle == 6) {if ((h >= 0 && h <= 5) || (h >= 12 && h <= 17)) { st = 1;} else { st = 0; }}
  if (toggle == 8) {if ((h >= 0 && h <= 7) || (h >= 16 && h <= 23)) { st = 1;} else { st = 0; }}
  if (toggle == 1) {if (h >= 0 && h <= 11) { st = 1;} else { st = 0; }}
  if(st == 1) {digitalWrite(equip[0],HIGH); delay(100); digitalWrite(equip[1],LOW);}
  if(st == 0) {digitalWrite(equip[0],LOW); delay(100); digitalWrite(equip[1],HIGH);}
  }
*/

  if(mode==1)
  {
    Serial.println("In timer");
    int st;int h;//tmElements_t tm;
  
    h=hour(); //minute();//{h=tm.Hour;}//
    //Serial.println(h);
    delay(200);
    if (toggle == 4) {if ((h >= 0 && h <= 3) || (h >= 8 && h <= 11) || (h >= 16 && h <= 19)) { st = 1;} else { st = 0; }}
    if (toggle == 6) {if ((h >= 0 && h <= 5) || (h >= 12 && h <= 17)) { st = 1;} else { st = 0; }}
    if (toggle == 8) {if ((h >= 0 && h <= 7) || (h >= 16 && h <= 23)) { st = 1;} else { st = 0; }}
    if (toggle == 1) {if (h >= 0 && h <= 11) { st = 1;} else { st = 0; }}
    
    if(((st == 1) && (present_state==0)) || ((st == 1) && (first_time==0))) 
    {
      first_time=1;flag0=1;flag1=0;present_state=1;
      digitalWrite(equip[0],flag0); delay(2000); digitalWrite(equip[1],flag1);
      delay(1000);
      sendMessage("AC1 is ON and AC2 is OFF");
      status_display(1);
      delay(1000);    
    }
    if(((st == 0) && (present_state==1)) || ((st == 0) && (first_time==0))) 
    {
      first_time=1;flag0=0;flag1=1;present_state=0;
      digitalWrite(equip[0],flag0); delay(2000);digitalWrite(equip[1],flag1);
      delay(1000);
      status_display(1);
      sendMessage("AC1 is OFF and AC2 is ON");
      delay(1000);
    }
  }
 
  if(mehar.available())
  {
    int inputChar;char data[200];
    int j=0;while(mehar.available()){data[j]=mehar.read();Serial.println(data[j]);j++;delay(1);} //Accumulate data

    for(int i=0;i<=j;i++ )
    {
      if((data[i]=='#') && (data[i+1]=='1') && (data[i+2]=='O') && (data[i+3]=='N'))
      {
        
        if(mode==1){mode_set(0);}
        flag0=1;
        rcv_msg();
        Serial.println(" OPTIMUS is happy to save your time AC 1 is ON ");
        
        EEPROM.write(2,flag0);
        delay(500);
        digitalWrite(equip[0],flag0);
        //digitalWrite(13,HIGH);
        delay(500);
        status_display(0);
        delay(500);
        sendMessage("OPTIMUS is happy to save your time AC 1 is ON");
        deleteMessage();
        delay(1000);
      }
      if((data[i]=='#') && (data[i+1]=='1') && (data[i+2]=='O') && (data[i+3]=='F') && (data[i+4]=='F'))
      {
        if(mode==1){mode_set(0);}
        flag0=0;
        rcv_msg();
        Serial.println("OPTIMUS is happy to save your time AC 1 is OFF");
        
        EEPROM.write(2,0);
        delay(500);
        digitalWrite(equip[0],flag0);
        //digitalWrite(13,LOW);
        delay(500);
        status_display(0);
        delay(500);
        sendMessage("OPTIMUS is happy to save your time AC 1 is OFF");
        deleteMessage();
        delay(1000);
      }
      
      if((data[i]=='#') && (data[i+1]=='2') && (data[i+2]=='O') && (data[i+3]=='N'))
      {
        if(mode==1){mode_set(0);}
        flag1=1;
        rcv_msg();
        Serial.println("OPTIMUS is happy to save your time AC 2 is ON");
        EEPROM.write(3,flag1);
        delay(500);
        digitalWrite(equip[1],flag1);
        delay(500);
        status_display(0);
        delay(500);
        sendMessage("OPTIMUS is happy to save your time AC 2 is ON");
        deleteMessage();
        delay(1000);
      }
      if((data[i]=='#') && (data[i+1]=='2') && (data[i+2]=='O') && (data[i+3]=='F') && (data[i+4]=='F'))
      {
        if(mode==1){mode_set(0);}
        flag1=0;
        rcv_msg();
        Serial.println("OPTIMUS is happy to save your time AC 2 is OFF");
        delay(500);
        EEPROM.write(3,flag1);
        digitalWrite(equip[1],flag1);
        delay(500);
        status_display(0);
        delay(500);
        sendMessage("OPTIMUS is happy to save your time AC 2 is OFF");
        deleteMessage();
        delay(1000);
      }
      
      if((data[i]=='#') && (data[i+1]=='T') && (data[i+2]=='T') && ( (data[i+3]=='4') || (data[i+3]=='6') || (data[i+3]=='8') || ( (data[i+3]=='1')) &&(data[i+4]=='2')))
      {
        if(mode==0){mode_set(1);}
        char t;
        t = data[i+3];
        toggle = t-48;
        first_time=0;
        EEPROM.write(0,toggle);
        rcv_msg();
        Serial.println("Toggle is: ");
        Serial.println(toggle);
        Serial.println(" Mode Set ");
        status_display(1);
        Serial.println("Waheguru tera Shukr h Mode set");
        delay(500);
        delay(10000);
        if(toggle==4){sendMessage("OPTIMUS Says: Mode Set to 4 hrs");}
        if(toggle==6){sendMessage("OPTIMUS Says: Mode Set to 6 hrs");}
        if(toggle==8){sendMessage("OPTIMUS Says: Mode Set to 8 hrs");}
        if(toggle==1){sendMessage("OPTIMUS Says: Mode Set to 12 hrs");}
        //sendMessage("OPTIMUS Says: New Mode Set");
        deleteMessage();
        delay(5000); 
      }
      
      if((data[i]=='#') && (data[i+1]=='S') && (data[i+2]=='T') && (data[i+3]=='A') && (data[i+4]=='T') && (data[i+5]=='U') && (data[i+6]=='S'))
      {
        rcv_msg();
        Serial.println("In status");
        if((flag0==0) && (flag1==0)){sendMessage("Both ACs are OFF");}
        if((flag0==1) && (flag1==0)){sendMessage("AC1 is ON and AC2 is OFF");}
        if((flag0==0) && (flag1==1)){sendMessage("AC1 is OFF and AC2 is ON");}
        if((flag0==1) && (flag1==1)){sendMessage("Both ACs are ON");}
        status_display(mode);
      }  

    }
  }
  
}

