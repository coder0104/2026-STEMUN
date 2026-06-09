#include <OneWire.h>
#include <DallasTemperature.h>

const int trigPin = 2;       
const int echoPin = 3;       
const int turbidityPin = A0; 
const int ONE_WIRE_BUS = A1; 
const int tdsPin = A2;       

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

const float TANK_DEPTH = 20.0; 
const int TURB_CLEAN = 850;    
const int TURB_DIRTY = 300;    
const int TDS_CLEAN = 64;      
const int TDS_DIRTY = 700;     

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(turbidityPin, INPUT);
  pinMode(tdsPin, INPUT);
  
  sensors.begin(); 

  Serial.println("=============================================================");
  Serial.println(" SBM-U 2.0 ODF++ 인증 기준 저비용 정화조 모니터링 시스템 ");
  Serial.println("=============================================================");
}

void loop() {
  long duration;
  float distance;
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH, 30000);
  distance = duration * 0.034 / 2;

  float fillLevel = ((TANK_DEPTH - distance) / TANK_DEPTH) * 100.0;
  fillLevel = constrain(fillLevel, 0, 100);

  int turbidityRaw = analogRead(turbidityPin);
  float turbidityScore = map(turbidityRaw, TURB_CLEAN, TURB_DIRTY, 0, 100);
  turbidityScore = constrain(turbidityScore, 0, 100);

  int tdsRaw = analogRead(tdsPin);
  float tdsScore = map(tdsRaw, TDS_CLEAN, TDS_DIRTY, 0, 100);
  tdsScore = constrain(tdsScore, 0, 100);

  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);

  float finalScore = (fillLevel * 0.60) + (turbidityScore * 0.25) + (tdsScore * 0.15);

  Serial.print("[채움률]: "); Serial.print(fillLevel, 1); Serial.print("%");
  Serial.print(" | [탁도점수]: "); Serial.print(turbidityScore, 0);
  Serial.print(" | [TDS생값]: "); Serial.print(tdsRaw);
  Serial.print(" | [TDS점수]: "); Serial.print(tdsScore, 0);
  
  Serial.print(" | [수온]: ");
  if (temperatureC == -127.0) {
    Serial.print("오류");
  } else {
    Serial.print(temperatureC, 1); Serial.print("°C");
  }

  Serial.print(" => ⭐ [수거 필요도]: ");
  Serial.print(finalScore, 1);
  Serial.print("점 -> ");

  if (finalScore >= 80.0) {
    Serial.println("🔴 [🚨 긴급 수거]"); 
  } else if (finalScore >= 60.0 && finalScore < 80.0) {
    Serial.println("🟠 [빠른 수거 필요]"); 
  } else if (finalScore >= 40.0 && finalScore < 60.0) {
    Serial.println("🟡 [정기 수거 대상]"); 
  } else {
    Serial.println("🟢 [정상 (모니터링)]"); 
  }

  delay(1000); 
}