
#define BLYNK_TEMPLATE_ID "TMPL3zpm_7DtO"
#define BLYNK_TEMPLATE_NAME "Water Level Monitor"
#define BLYNK_AUTH_TOKEN "8QA-UhLL99quafLEL_jjXUtkVzwY5diy"



#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// WiFi credentials
char ssid[] = "realme8";      // 🔹 Replace with your WiFi or Hotspot name
char pass[] = "12341234";  // 🔹 Replace with your WiFi password

// Ultrasonic Sensor Pins
#define TRIG_PIN D5
#define ECHO_PIN D6

// Tank height (in cm) — distance from sensor to bottom when EMPTY
#define TANK_HEIGHT_CM 11.71

BlynkTimer timer;

void setup() {
  Serial.begin(9600);
  Serial.println("Connecting to WiFi...");

  // Connect to Blynk Cloud
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Measure every 2 seconds
  timer.setInterval(2000L, measureWaterLevel);
}

void loop() {
  Blynk.run();
  timer.run();
}

void measureWaterLevel() {
  // Trigger ultrasonic pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read echo time
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  float distance = duration * 0.034 / 2;  // Convert to cm

  // Constrain distance to tank height
  if (distance > TANK_HEIGHT_CM) distance = TANK_HEIGHT_CM;
  if (distance < 0) distance = 0;

  // Calculate percentage (100% = full, 0% = empty)
  int levelPercent = map(distance, TANK_HEIGHT_CM, 0, 0, 100);

  // Print readings to Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm  |  Water Level: ");
  Serial.print(levelPercent);
  Serial.println("%");

  // Send data to Blynk (V0)
  Blynk.virtualWrite(V0, levelPercent);
}
