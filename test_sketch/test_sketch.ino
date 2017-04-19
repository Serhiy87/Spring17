void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
Serial3.begin(9600);
pinMode(PC13,OUTPUT);
Serial3.println("AT");
}

void ledBlink(uint16_t timeout){
  static uint8_t ledState=0;
  static uint32_t Timer=0;
  switch(ledState){
    case 0:
        if((millis()-Timer)>timeout){
          digitalWrite(PC13, LOW);
          Timer=millis();
          ledState=1; 
        }
        break;
    case 1:
        if((millis()-Timer)>timeout){
          digitalWrite(PC13, HIGH);
          Timer=millis();
          ledState=0; 
        }
        break;
    }
  }

void SerialTransparent(){
  static uint8_t state=0;
  static uint32_t Timer=0;
  switch(state){
    case 0:
        if(Serial3.available()){
          Serial.println("S3_say:");
          Timer=millis();
          state=1;
          }
        break;
    case 1:
        if((millis()-Timer)>100){
          while(Serial3.available()>0){
            char c=Serial3.read();
          Serial.println(c);
          }
          state=0; 
        }
        break;
    }
  }


void loop() {
  // put your main code here, to run repeatedly:
  ledBlink(1000);
if (Serial3.available())
    Serial.write(Serial3.read());
  if (Serial.available())
    Serial3.write(Serial.read());  
}
