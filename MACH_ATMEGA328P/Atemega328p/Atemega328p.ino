#define LED 8
#define RELAY_1 9
#define BTN_LED 2
#define BTN_RELAY1 3
#define RELAY_2 10
#define LM35_SS A0
bool Status_Led = 0;
bool Status_Relay1 = 0;
unsigned long Last_Time =0;
int LM35_Temp=0;

//Su dung Interrupt cho 2 btn_led, btn_relay1
/*
void interrupt_led(){
    //LED 
   unsigned long currentime = millis();
   if((currentime - Last_time) > 50){
     if( digitalRead(BTN_LED) == 0){   
         if(Status_Led == 0 ) Status_Led=1;
         else if (Status_Led==1) Status_Led =0;
         digitalWrite(LED,Status_Led);
      Last_time = currentime;
   }
   detachInterrupt(digitalPinToInterrupt(BTN_LED));
   }
}*/

/*void interrupt_relay1(){
    // RELAY1
   unsigned long currentime = millis();
   if((currentime - Last_Time) > 50){
     if( digitalRead(BTN_RELAY1) == 0){   
         if(Status_Relay1 == 0 ) Status_Relay1=1;
         else if (Status_Relay1==1) Status_Relay1 =0;
         digitalWrite(RELAY_1,Status_Relay1);
         Last_Time = currentime;
   }
   detachInterrupt(digitalPinToInterrupt(BTN_RELAY1));
   }
}
*/

void setup() {
  // put your setup code here, to run once:
   pinMode(LED, OUTPUT);
   pinMode(RELAY_1,OUTPUT);
   pinMode(RELAY_2,OUTPUT);
   pinMode(LM35_SS, INPUT);
   pinMode(BTN_LED,INPUT);
   pinMode(BTN_RELAY1,INPUT);
   //attachInterrupt(digitalPinToInterrupt(BTN_LED), interrupt_led, FALLING);       // Cau hinh Interrupt (neu su dung Interrupt cho button)
  //attachInterrupt(digitalPinToInterrupt(BTN_RELAY1), interrupt_relay1, FALLING);  // Cau hinh Interrupt (neu su dung Interrupt cho button)

}

void loop() {
   /* if(millis() - last_time >100){
      attachInterrupt(digitalPinToInterrupt(BTN_LED), interrupt_led, FALLING);
      attachInterrupt(digitalPinToInterrupt(BTN_RELAY1), interrupt_relay1, FALLING);
   }*/
   
   // kiểm tra trạng thái và chống dội cho nút nhấn led
   if( digitalRead(BTN_LED) == 0){ 
      delay(50);
      if(digitalRead(BTN_LED) == 0){  
         if(Status_Led == 0 ) Status_Led=1;
         else if (Status_Led==1) Status_Led =0;
         digitalWrite(LED,Status_Led);
      }
      while(digitalRead(BTN_LED) == 0);
      delay(50);
   }

   // kiểm tra trạng thái và chống dội cho nút nhấn relay1
    if( digitalRead(BTN_RELAY1) == 0){   
      delay(50);
         if(digitalRead(BTN_RELAY1) == 0){
         if(Status_Relay1 == 0 ) Status_Relay1=1;
         else if (Status_Relay1==1) Status_Relay1 =0;
         digitalWrite(RELAY_1,Status_Relay1);
         }
      while(digitalRead(BTN_RELAY1) == 0);
      delay(50);
   }

   // Đọc giá trị cảm biến LM35 và xử lý giá trị 
   float value = analogRead(LM35_SS);
   LM35_Temp = (value*5.0*100.0)/ (1024.0*2.0);
   
   //Kiểm tra giá trị nhiệt độ LM35 -> Điểu khiển Relay
   if(LM35_Temp >=37) digitalWrite(RELAY_2, HIGH);
   else digitalWrite(RELAY_2, LOW);
  
}
