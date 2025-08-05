#define MOTOR_PWM 6  // Chân điều khiển tốc độ
#define MOTOR_A 5  // Chân điều khiển hướng
#define MOTOR_B 4
#define ENCODER_A 2
#define ENCODER_B 3

#define IN1  8
#define IN2  9
#define IN3  10
#define IN4  11

// DONG CO DC
// 1 vong 104 - 105 xung 
volatile int pulse_count = 0;
int number_rounds = 5;
int rounds =0;
String command = "";
String direction_motor="";


// DONG CO DC ENCODER 
// HAM NGAT 
void  encoder_ISR() {
  pulse_count++;
  if (pulse_count == 112) {
      rounds ++;
      Serial.println(rounds);
      Serial.println(pulse_count);
      pulse_count = 0;
      if(number_rounds == rounds ){
        digitalWrite(MOTOR_A, LOW); 
        digitalWrite(MOTOR_B, LOW);
      }
  }
}

// DONG CO STEP
// Số bước của động cơ (1 vòng = 4096 bước với hộp số 64:1)
const int step_full = 2048; 
const int step_half = 4096;
const int stepDelay = 5; // Thời gian trễ giữa các bước (ms)

long degree = 0;
long function_degree=0;


void full_stepMotor(int step, int direction) {
  int state = (direction == 1) ? step % 4 : (3 - (step % 4));
  switch (state) {
    case 0:  
      digitalWrite(IN1, HIGH); digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);  digitalWrite(IN4, LOW);
      break;
    case 1:  
      digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
      break;
    case 2:  
      digitalWrite(IN1, LOW);  digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH); digitalWrite(IN4, HIGH);
      break;
    case 3:  
      digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
      break;
  }
}

void half_stepMotor(int step, int direction) {
  int state = (direction == 1) ? step % 8 : (7 - (step % 8));
  switch (state) {
    case 0:  
      digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);  digitalWrite(IN4, LOW);
      break;
    case 1:  
      digitalWrite(IN1, HIGH);  digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
      break;
    case 2:  
      digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
      break;
    case 3:  
      digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH);  digitalWrite(IN4, LOW);
      break;
    case 4:  
      digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);  digitalWrite(IN4, LOW);
      break;
    case 5:  
      digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);  digitalWrite(IN4, HIGH);
      break;
    case 6:  
      digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
      break;
    case 7:  
      digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
      break;      
  }
}

void rotate_full(int steps, int direction) {
  for (int i = 0; i <steps; i++) {
    full_stepMotor(i,direction);
    delay(stepDelay);
  }
}

void rotate_half(int steps, int direction) {
  for (int i = 0; i <steps ; i++) {
    half_stepMotor(i,direction);
    delay(stepDelay);
  }
}



void setup() {
  Serial.begin(9600);
  pinMode(MOTOR_PWM, OUTPUT);
  pinMode(MOTOR_A, OUTPUT);
  pinMode(MOTOR_B, OUTPUT);
  pinMode(ENCODER_A, INPUT_PULLUP);
  pinMode(ENCODER_B, INPUT_PULLUP);
  analogWrite(MOTOR_PWM, 52);  // Điều chỉnh tốc độ (0 - 255)
  digitalWrite(MOTOR_A, LOW); // Hướng quay
  digitalWrite(MOTOR_B, LOW); // Hướng quay
  attachInterrupt(digitalPinToInterrupt(ENCODER_A), encoder_ISR, RISING);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop() {  
  if(Serial.available()){
     command = Serial.readStringUntil('\n');
     command.trim();
     // DONG CO DC
     if(command =="STOP"){
        digitalWrite(MOTOR_A, LOW); // Hướng quay
        digitalWrite(MOTOR_B, LOW); // Hướng quay
        pulse_count =0;
        rounds =0;
     } 
     else if(command.startsWith("DC FWD")){
        number_rounds = command.substring(7).toInt();
        digitalWrite(MOTOR_A, HIGH); // Hướng quay
        digitalWrite(MOTOR_B, LOW); // Hướng quay
        pulse_count =0;
        rounds =0;
     }
     else if(command.startsWith("DC REV")){
        number_rounds = command.substring(7).toInt();
        digitalWrite(MOTOR_A, LOW); // Hướng quay
        digitalWrite(MOTOR_B, HIGH); // Hướng quay
        pulse_count =0;
        rounds =0;
     } 
     else if(command.startsWith("STEP FWD")){
      degree = command.substring(9).toInt();
      function_degree = degree * 2048 /360;
      rotate_full(function_degree,1);
     }
     else if(command.startsWith("STEP REV")){
      degree = command.substring(9).toInt();
      function_degree = degree * 2048 /360;
      Serial.println(function_degree);
      rotate_full(function_degree,0);  
     }
     Serial.println(command);
  }
}
