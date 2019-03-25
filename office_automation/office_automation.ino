#define LDRpin A1
int tempPin=A0;
int smoke_read=A5;
int ir_sensor=2;
int bulb_read = 5;
int fan=6;
int buzzer=10;
int redLed = 12;
float tempMin=25;
float tempMax=80;
int smoke_thresh=80;
int manual_value;
int fan_manual;
int bulb_manual;
int fanSpeed=0;
int LDRvalue=0;
float temp;
String command = "auto_on";
char k;
void setup() {
  pinMode(fan,OUTPUT);
  pinMode(ir_sensor,INPUT);
  pinMode(tempPin,INPUT);
  pinMode(bulb_read,OUTPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(smoke_read,INPUT); 
  Serial.begin(9600); 
}

void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x07; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}
void loop() {
  LDRvalue=analogRead(LDRpin);
  int smoke_value=analogRead(smoke_read);
  temp=analogRead(tempPin);
  float val=(temp*5000.0)/1024.0;
  float mv=(val/10.0);
  Serial.println(LDRvalue);
  Serial.print("Smoke value");
  Serial.println(smoke_value);
  Serial.print("Temp=");
  Serial.println(mv);
  setPwmFrequency(5, 1024);
  /*if(Serial.available())
  {
        command = Serial.read();
  }*/
  if(digitalRead(ir_sensor)==LOW)
  {
  Serial.println("Motion Detected.");
  Serial.print("LDR Reading: ");
  Serial.print(LDRvalue);
  }
  if(command=="auto_on" && digitalRead(ir_sensor)==LOW)
  {
        if (LDRvalue > 1000) { analogWrite(bulb_read, 0)  ; Serial.println("dutycycle is 0%");}
        else if (LDRvalue>700 && LDRvalue<1001) { analogWrite(bulb_read,64 )  ; Serial.println("dutycycle is 25%");}
        else if(LDRvalue>300 && LDRvalue<401) { analogWrite(bulb_read, 127)  ; Serial.println("dutycycle is 50%");}
        else if (LDRvalue>400 && LDRvalue<501) { analogWrite(bulb_read, 191)  ; Serial.println("dutycycle is 75%");}
        else if (LDRvalue<500) { analogWrite(bulb_read, 255)  ; Serial.println("dutycycle is 100%");}
  }
  if(Serial.available()>0 || command=="manual")                           
  {     
        Serial.println("Prefrence:");     
        k=Serial.read();
        Serial.print("k:");
        Serial.print(k);
        if(k=='0')
        {
                    bulb_manual=0;
        Serial.print("Manual Value(BULB): ");
        Serial.println(0);
        analogWrite(bulb_read, bulb_manual);
        }
        if(k=='1')
        {
                    bulb_manual=64;
        Serial.print("Manual Value(BULB): ");
        Serial.println(64);
        analogWrite(bulb_read, bulb_manual);
        }
         if(k=='2')
        {
                bulb_manual=127;
        Serial.print("Manual Value(BULB): ");
        Serial.println(127);
        analogWrite(bulb_read, bulb_manual);
        }
        if(k=='3')
        {
                    bulb_manual=191;
        Serial.print("Manual Value(BULB): ");
        Serial.println(191);
        analogWrite(bulb_read, bulb_manual);
        }
        if(k=='4')
        {
          bulb_manual=255;
        Serial.print("Manual Value(BULB): ");
        Serial.println(255);
        analogWrite(bulb_read, bulb_manual);
        }
        if(k=='5')
        {
        fan_manual=0;
        Serial.print("Manual Value(FAN): ");
        Serial.println(fan_manual);
        analogWrite(6, fan_manual); 
            //  delay(2000);
        }
        if(k=='6')
        {
        fan_manual=255;
        Serial.print("Manual Value(FAN): ");
        Serial.println(fan_manual);
        analogWrite(6, fan_manual); 
          // delay(2000);
        }
        if(k=='7')
        {
        fan_manual=127;
        Serial.print("Manual Value(FAN): ");
        Serial.println(fan_manual);
        analogWrite(6, fan_manual); 
        // delay(2000);
        }
        if(k=='8')
        {
        fan_manual=191;
        Serial.print("Manual Value(FAN): ");
        Serial.println(fan_manual);
        analogWrite(6, fan_manual); 
        //delay(2000);
        }
       /* Serial.print("Manual Value(BULB): ");
        Serial.println(manual_value);
        analogWrite(bulb_read, manual_value); */
        command="manual";   
  } 
  if(command=="auto_on" && mv<tempMin && digitalRead(ir_sensor)==LOW)
  {
  Serial.print("Fan Speed");
  Serial.print(fanSpeed);
  analogWrite(fan,fanSpeed);
  }
  else if(command=="auto_on" && (mv >= tempMin) && (mv <= tempMax) && digitalRead(ir_sensor) == LOW) //if temperature is higher than the minimum range
  {
  fanSpeed = map(mv, tempMin, tempMax, 32, 255); // the actual speed of fan
  Serial.print("Fan speed ");
  Serial.println(fanSpeed);
  analogWrite(fan, fanSpeed);
  }
  else if(command=="auto_on" && mv>tempMax && digitalRead(ir_sensor) == LOW)
  {
  Serial.print("Fan speed ");
  Serial.println("255");
  analogWrite(fan, 255);
  }
  if(smoke_value > smoke_thresh)
  {
  digitalWrite(redLed, HIGH);
  tone(buzzer, 1000, 200);
  }
  else
  {
  digitalWrite(redLed, LOW);
  noTone(buzzer);
  }
  delay(100);   
}
