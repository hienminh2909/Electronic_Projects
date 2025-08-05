#include <DHT.h>
#include <LiquidCrystal_I2C.h>

const int trig = 8;     
const int echo = 9;     

LiquidCrystal_I2C lcd(0X27,20,4); 
#define DHTPIN 2
#define DHTTYPE DHT22

#define num_readings 10 //tính trung bình cho dht22 (lọc trung bình)

DHT dht(DHTPIN, DHTTYPE);

float tempReadings[num_readings]; 
float humReadings[num_readings];  
int readIndex = 0;              

void setup() {
  Serial.begin(9600);
  dht.begin();
  
  for (int i = 0; i < num_readings; i++) {
    tempReadings[i] = 0;
    humReadings[i] = 0;
  }

   lcd.init();
   lcd.backlight();
    pinMode(trig,OUTPUT);   
    pinMode(echo,INPUT);    
    lcd.clear();
}

void loop() {
  
  //DHT22
  float humi = dht.readHumidity();
  float temp = dht.readTemperature();

  if (isnan(humi) || isnan(temp)) {
    Serial.println("Không thể đọc từ cảm biến DHT22!");
    return;
  }

 
  tempReadings[readIndex] = temp;
  humReadings[readIndex] = humi;

  
  readIndex = (readIndex + 1) % num_readings;

  // Tính trung bình để lọc nhiễu (bộ lọc trung bình)
  float tempSum = 0, humSum = 0;
  for (int i = 0; i < num_readings; i++) {
    tempSum += tempReadings[i];
    humSum += humReadings[i];
  }
  float avgTemp = tempSum / num_readings;
  float avgHum = humSum / num_readings;

  
  //HC_SR04
  unsigned long duration; 
  unsigned int distance;           
    
    /* Phát xung từ chân trig */
    digitalWrite(trig,0);   
    delayMicroseconds(2);
    digitalWrite(trig,1);   
    delayMicroseconds(5);   
    digitalWrite(trig,0);  
    
    /* Tính toán thời gian */  
    duration = pulseIn(echo,HIGH);  
    distance = int(duration/2/29.5);

    lcd.setCursor(0,0);
    lcd.print("khoang cach: ");
    lcd.print(distance);
    lcd.print("  cm  ");
    
    lcd.setCursor(0,2);
    lcd.print("Nhiet do: ");
    lcd.print(avgTemp);
    lcd.write(223);
    lcd.print("C");

    lcd.setCursor(0,3);
    lcd.print("Do am: ");
    lcd.print(avgHum);
    lcd.print(" %");

    delay(100);
}