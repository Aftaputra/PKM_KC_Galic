#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>
#include <cmath>

const int ledPin = 13;  // LED pin
Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);

float I0_empty = 1268.0, I0_empty_new = 0.0, I0_blank = 1196.0, A_blank = 0.0;
float I_sample[10] = {0.0};
float A_sample[10] = {0.0};

void displaySensorDetails() {
  sensor_t sensor;
  tsl.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print("Sensor:       "); Serial.println(sensor.name);
  Serial.print("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" lux");
  Serial.print("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" lux");
  Serial.print("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" lux");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void configureSensor() {
  // tsl.enableAutoRange(true); // Auto-gain ... switches automatically between 1x and 16x
  tsl.setGain(TSL2561_GAIN_1X); 
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS); // fast but low resolution

  Serial.println("------------------------------------");
  Serial.print("Gain:         "); Serial.println("Auto");
  Serial.print("Timing:       "); Serial.println("13 ms");
  Serial.println("------------------------------------");
}

void setup() {
  Serial.begin(9600);
  Serial.println("Light Sensor Test"); Serial.println(""); 
  pinMode(ledPin, OUTPUT);  // Set the LED pin as output
  if(!tsl.begin()) {
    Serial.print("Ooops, no TSL2561 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  displaySensorDetails();
  configureSensor();
}

void measureLight(float &lightValue, const char* description) {
  digitalWrite(ledPin, HIGH);  // Turn on the LED
  Serial.println("LED is ON");
  delay(750);
  sensors_event_t event;
  tsl.getEvent(&event);
  lightValue = event.light;
  Serial.print(description); Serial.print(": "); Serial.println(lightValue);     
  digitalWrite(ledPin, LOW);  // Turn off the LED
  Serial.println("LED is OFF");
}

void measureAbsorbance(float I_sample[], float A_sample[], int index) {
  measureLight(I_sample[index], ("I_sample " + String(index + 1)).c_str());
  float calibrationFactor = abs(I0_empty - I0_empty_new);
  if (I0_empty > I0_empty_new){
    I_sample[index] + calibrationFactor;
    I0_blank + calibrationFactor;

  } else if (I0_empty < I0_empty_new){
    I_sample[index] - calibrationFactor;
    I0_blank - calibrationFactor;
  } else {
    I_sample[index] = I_sample[index];
    I0_blank = I0_blank;
  }
  Serial.print("Adjusted I_sample "); Serial.print(index + 1); Serial.print(" : "); Serial.println(I_sample[index]);
  A_sample[index] = abs(-log(I_sample[index] / I0_blank) - A_blank);
  Serial.print("A_sample "); Serial.print(index + 1); Serial.print(" : "); Serial.println(A_sample[index], 3);
}

void loop() {  
  if (Serial.available() > 0) {
    char input = Serial.read();
    if (input == 'y' || input == 'Y') {
      measureLight(I_sample[0], "Light"); // You can change index as needed
    } else if (input == 'e') { // Measure I0_empty
      measureLight(I0_empty, "I0_empty");
    } else if (input == 'b') { // Measure I0_blank
      measureLight(I0_blank, "I0_blank");
    } else if (input == 'n') { // Measure new I0_empty
      measureLight(I0_empty_new, "New I0_empty");
    } else if (input >= '1' && input <= '9') { // Measure I_sample 1 to 9
      int sampleIndex = input - '1'; // Convert char to int index (0 to 8)
      measureAbsorbance(I_sample, A_sample, sampleIndex);
    } else if (input == '=') { // Measure A_blank and pr=int all A_sample values
      A_blank = abs(-log(I0_blank / I0_empty));
      Serial.print("A_blank: "); Serial.println(A_blank, 3);
      for (int i = 0; i < 10; i++) {
        Serial.print("A_sample "); Serial.print(i + 1); Serial.print(" = "); Serial.println(A_sample[i], 3);
      }
    }
  }
}
