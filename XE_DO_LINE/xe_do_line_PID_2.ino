#define EnA 6  // PWM bánh phải
#define EnB 5  // PWM bánh trái
#define InA 9
#define InB 10
#define InC 7
#define InD 8
#define SENSOR_PORT PINC

const int sensor_pins[5] = {A4, A3, A2, A1, A0};//cảm biến từ trái sang phải

//THÔNG SỐ PID 
int Kp =20.0;   
int Kd = 51.0;  
int Ki = 0.001;
float error, previous_error = 0, integral = 0;
int rightSpeedPwm = 0, leftSpeedPwm = 0;

// CÀI ĐẶT TỐC ĐỘ
#define MAX_SPEED  240  
#define MIN_SPEED  0  
#define STRAIGHT_SPEED  60  // Tốc độ cơ bản khi đi thẳng
#define TURN_SPEED 40   // Tốc độ cơ bản khi đi trong khúc cua
int  BASE_SPEED = STRAIGHT_SPEED; // Tốc độ cơ bản khởi tạo
float weights[5] = {-5, -3, 0, 3, 5}; //Trọng số cảm biến từ trái sang phải


// Thời gian kiểm tra cua
unsigned long turnStartTime = 0; // Thời điểm bắt đầu phát hiện cua
unsigned long straightStartTime = 0; // Thời điểm bắt đầu phát hiện đi thẳng
const unsigned long TURN_THRESHOLD = 0; // Ngưỡng thời gian xác nhận cua (ms)
const unsigned long STRAIGHT_THRESHOLD = 300; // Ngưỡng thời gian xác nhận hết cua (ms)
bool Flag_Turning = false; // Trạng thái gặp khúc  cua

#define DEBUG

//HÀM SETUP 
void setup() {
    pinMode(InA, OUTPUT);
    pinMode(InB, OUTPUT);
    pinMode(InC, OUTPUT);
    pinMode(InD, OUTPUT);
    // mặc định động cơ đi thẳng không đi lùi 
    digitalWrite(InB, HIGH);
    digitalWrite(InA, LOW);
    digitalWrite(InD, LOW);
    digitalWrite(InC, HIGH);
    
    pinMode(EnA, OUTPUT);
    pinMode(EnB, OUTPUT);
    
    for (int i = 0; i < 5; i++) {
        pinMode(sensor_pins[i], INPUT);
    }
    
    Serial.begin(9600); 
}

//  HÀM ĐỌC CẢM BIẾN 
float getSensor() {
    int sensorValues = ~SENSOR_PORT & 0x1F;  // Đọc 5 bit thấp A0 - A4 , giá trị cảm biến gốc 
    // (đen = 0, trắng = 1), đảo lại(đen = 1, trắng = 0)

    float sum = 0, sumWeights = 0;

    for (int i = 0; i < 5; i++) {
        int value = (sensorValues >> (4-i)) & 1;
        sum += value * weights[i];
        sumWeights += value;
    }
    if (sumWeights > 0) {
        error = sum / sumWeights;
        
    } else {
        error = previous_error; // Giữ hướng trước đó
    }

    return error;
}

//  HÀM TÍNH PID 
float calculate_pid() {
    integral += error;
    float pid = (Kp * error) + (Kd * (error - previous_error)) + (Ki * integral);
    previous_error = error; 
    return pid;
}

// HÀM ĐIỀU KHIỂN ĐỘNG CƠ
void control_motor(int left, int right) {
    analogWrite(EnA, right);
    analogWrite(EnB, left);
}

// HÀM LOOP 
void loop() {
    error = getSensor(); // Đọc dữ liệu cảm biến
    float pid = calculate_pid(); // Tính toán PID
    

    // Xử lý khúc cua khi đi với tốc độ nhanh 
    //unsigned long currentTime = millis();
    if (abs(error) !=0 ) { // Lỗi lớn => có thể đang cua        
        Flag_Turning = true; // Xác nhận đang cua
        BASE_SPEED = TURN_SPEED; // Giảm tốc độ cơ bản       
        straightStartTime = 0; // Reset thời gian đi thẳng
    } 
    else { // Lỗi nhỏ => có thể đã hết cua
      /* if (Flag_Turning) { // Nếu đang cua trước đó
            if (straightStartTime == 0) {
                straightStartTime = currentTime; // Bắt đầu đếm thời gian đi thẳng
            }
            if (currentTime - straightStartTime >= STRAIGHT_THRESHOLD) {
                Flag_Turning = false; // Xác nhận hết cua
                BASE_SPEED = STRAIGHT_SPEED; // Tăng tốc độ cơ bản ở những đoạn đường
            }
        }
        turnStartTime = 0; // Reset thời gian cua
        */
        BASE_SPEED = STRAIGHT_SPEED;
    }
    

    // Tính tốc độ dựa trên PID và giới hạn 
    if(pid < 0){
      if(pid<-80){
        leftSpeedPwm = (BASE_SPEED + pid) -10 ;
        rightSpeedPwm = (BASE_SPEED - pid) + 10;//
      }
      else{
        leftSpeedPwm = (BASE_SPEED + pid) -10 ;
        rightSpeedPwm = (BASE_SPEED - pid) + 10;//
      }
    }
    else if(pid>0)
    {
      if(pid>80){
        leftSpeedPwm = (BASE_SPEED + pid) + 10 ;//
        rightSpeedPwm = (BASE_SPEED - pid) -10  ;
      }
      else{
        leftSpeedPwm = (BASE_SPEED + pid) + 10 ;//
        rightSpeedPwm = (BASE_SPEED - pid) -10  ;
      }
    }
    else{
      leftSpeedPwm = BASE_SPEED  ;
      rightSpeedPwm = BASE_SPEED  ;
    }
    // Giới hạn tốc độ
    leftSpeedPwm = constrain(leftSpeedPwm, MIN_SPEED, MAX_SPEED);
    rightSpeedPwm = constrain(rightSpeedPwm, MIN_SPEED, MAX_SPEED);

    // Điều khiển động cơ
    control_motor(leftSpeedPwm, rightSpeedPwm);

    
    #ifdef DEBUG
    Serial.print("Error: ");
    Serial.print(error);
    Serial.print(" | PID: ");
    Serial.print(pid);
    Serial.print(" | Left Speed: ");
    Serial.print(leftSpeedPwm);
    Serial.print(" | Right Speed: ");
    Serial.println(rightSpeedPwm);
    #endif
    //Serial.println(~SENSOR_PORT & 0x1F, BIN);
    
}