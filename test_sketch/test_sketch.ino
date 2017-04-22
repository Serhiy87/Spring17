
void GSM_println(String line){
  
  Serial3.println(line);
  Serial.println(line);
  }
#define GSM_RX_BUF_MAXNUM 255
#define GSM_RXSTRING_MAX_SIZE 20
char GSM_RX_BUF[GSM_RX_BUF_MAXNUM+1];
uint16_t GSM_RX_WritePointer;
uint16_t GSM_RX_ReadPointer;
uint16_t GSM_maxindex=0;

void writeChartoGSM_RX_BUffer(char c){
  if(GSM_RX_WritePointer<(GSM_RX_BUF_MAXNUM-1)){
        GSM_RX_BUF[GSM_RX_WritePointer]=c;
        if((c=='\n')&&(GSM_RX_WritePointer>(GSM_RX_BUF_MAXNUM-GSM_RXSTRING_MAX_SIZE))){
             GSM_maxindex=GSM_RX_WritePointer;
             GSM_RX_WritePointer=0;           
          }else{ GSM_RX_WritePointer++;}
    }
    else{
      GSM_maxindex=GSM_RX_WritePointer;
      GSM_RX_BUF[0]=c;
      GSM_RX_WritePointer=1;
      }
  }
uint8_t GetCharFromGSM_RX_Buffer(char *result){
      if(GSM_RX_WritePointer>=GSM_RX_ReadPointer){
        if(GSM_RX_WritePointer==GSM_RX_ReadPointer)
            {return 0;}
        else{
              *result=GSM_RX_BUF[GSM_RX_ReadPointer];
              GSM_RX_ReadPointer++;
            };
      }else{
          *result=GSM_RX_BUF[GSM_RX_ReadPointer];
          if(GSM_RX_ReadPointer==GSM_maxindex){
                GSM_RX_ReadPointer=0;
            }
        }
      
      return 1;
      }
  
void PrintGSM_RX_buffer(){
  Serial.println("StartBuffer:");
  /*for(int i=0;i<GSM_RX_BUF_MAXNUM;i++){
  Serial.print(GSM_RX_BUF[i]);}*/
  Serial.print(GSM_RX_BUF);
  Serial.println("EndBuffer:");
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
enum GSM_states{
  POWER_ON,WAIT_RDY,WAIT_CALL_READY, GSM_IDLE,RESET
  };
void GSM_APP(uint8_t event=0){
   static uint8_t state=0;
  static uint32_t Timer=0;
  if(event!=0){
    if(event==1){state=RESET;}
    }

  switch(state){
     case POWER_ON:{
         if((millis()-Timer)>1000)   
          {
            digitalWrite(PB1,LOW);
             Timer=millis();
            state=WAIT_RDY;
          }   }
        break;   
    case WAIT_RDY:
               if((millis()-Timer)>1000){
 
                Timer=millis();
                state=WAIT_CALL_READY;
               }
        break;
    case WAIT_CALL_READY:
        if((millis()-Timer)>10000){
                     // GSM_println("ATE0");   
          
          GSM_println("ATD+380633232300;");
          state=GSM_IDLE; 
          Timer=millis();
        }
        break;
   case GSM_IDLE:
        if((millis()-Timer)>10000){ 
          //state=RESET;
        }
        break;
    case RESET:

         //PrintGSM_RX_buffer();
         Timer=millis();
         digitalWrite(PB1,HIGH);
         state = POWER_ON;
        break;
    }
    //Serial.println(state);
  }

char a,b;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
Serial3.begin(9600);
pinMode(PC13,OUTPUT);
pinMode(PB1,OUTPUT);
digitalWrite(PB1,LOW);
GSM_APP(1);
               Serial3.println("AT");
}

void loop() {
  // put your main code here, to run repeatedly:
  ledBlink(1000);
  GSM_APP();
if (Serial3.available())
    {
      a=Serial3.read();
      writeChartoGSM_RX_BUffer(a);
      Serial.write(a);
      }
  if (Serial.available()){
    b=Serial.read();
    Serial3.write(b);
    }
}

