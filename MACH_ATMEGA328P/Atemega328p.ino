#define led 8
#define relay1 9
#define btn_led 2
#define btn_relay1 3
#define relay2 10
#define lm35 A0
bool status_led = 0;
bool status_relay1 = 0;
unsigned long last_time =0;
int temp=0;
/*
void interrupt_led(){
    //LED 
   unsigned long currentime = millis();
   if((currentime - last_time) > 50){
     if( digitalRead(btn_led) == 0){   
         if(status_led == 0 ) status_led=1;
         else if (status_led==1) status_led =0;
         digitalWrite(led,status_led);
      last_time = currentime;
   }
   detachInterrupt(digitalPinToInterrupt(btn_led));
   }
}*/

/*void interrupt_relay1(){
    // RELAY1
   unsigned long currentime = millis();
   if((currentime - last_time) > 50){
     if( digitalRead(btn_relay1) == 0){   
         if(status_relay1 == 0 ) status_relay1=1;
         else if (status_relay1==1) status_relay1 =0;
         digitalWrite(relay1,status_relay1);
         last_time = currentime;
   }
   detachInterrupt(digitalPinToInterrupt(btn_relay1));
   }
}
*/
void setup() {
  // put your setup code here, to run once:
   pinMode(led, OUTPUT);
   pinMode(relay1,OUTPUT);
   pinMode(relay2,OUTPUT);
   pinMode(lm35, INPUT);
   pinMode(btn_led,INPUT);
   pinMode(btn_relay1,INPUT);
   //attachInterrupt(digitalPinToInterrupt(btn_led), interrupt_led, FALLING);
  //attachInterrupt(digitalPinToInterrupt(btn_relay1), interrupt_relay1, FALLING);

}

void loop() {
  /* if(millis() - last_time >100){
      attachInterrupt(digitalPinToInterrupt(btn_led), interrupt_led, FALLING);
      attachInterrupt(digitalPinToInterrupt(btn_relay1), interrupt_relay1, FALLING);
   }*/
   
  
   if( digitalRead(btn_led) == 0){ 
      delay(50);
      if(digitalRead(btn_led) == 0){  
         if(status_led == 0 ) status_led=1;
         else if (status_led==1) status_led =0;
         digitalWrite(led,status_led);
      }
      while(digitalRead(btn_led) == 0);
      delay(50);
   }
    if( digitalRead(btn_relay1) == 0){   
      delay(50);
         if(digitalRead(btn_relay1) == 0){
         if(status_relay1 == 0 ) status_relay1=1;
         else if (status_relay1==1) status_relay1 =0;
         digitalWrite(relay1,status_relay1);
         }
      while(digitalRead(btn_relay1) == 0);
      delay(50);
   }
   // LM35
   float value = analogRead(lm35);
   temp = (value*5.0*100.0)/ (1024.0*2.0);
   if(temp >=37) digitalWrite(relay2, HIGH);
   else digitalWrite(relay2, LOW);
  
}
