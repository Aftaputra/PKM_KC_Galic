#include <TFT_eSPI.h>  // Include the TFT_eSPI library
#include <esp_sleep.h>
#include <ezButton.h>
#define BUTTON_PIN 14
#define DEEPSLEEP_DURATION 4  // Time in seconds to hold the button for sleep
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>
#include <cmath>

// Define encoder pins
#define outputA 13
#define outputB 25

int ledPin = 26;  // LED pin
int ledPin2 = 27;  // LED pin
Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_HIGH, 12345);
Adafruit_TSL2561_Unified tsl2 = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);
float I0_empty, I0_blank, A_blank;
float I_sample;
float A_sample;
float X[12]; // Deklarasi array X untuk menyimpan hasil pengukuran
int measurementIndex = 0; // Index untuk menyimpan hasil pengukuran ke array X

ezButton button(BUTTON_PIN);  // create ezButton object that attach to pin 14

TFT_eSPI tft = TFT_eSPI();  // Create an instance of the TFT_eSPI library

const int numOptions = 4;  // Number of menu options
int currentNumOptions = numOptions;  // Number of menu options
const char *options[numOptions] = {"Kalibrasi 1", "Kalibrasi 2", "Sensor 1", "Sensor 2"};
const char *kalibrate[3] = {"Baca Kosong", "Baca Blanko", "Kembali"};
float sensorValue = 0;
float blankValue = 0;

// Define icons as bitmaps (monochrome bitmaps)
// These are placeholders; you need to generate your own monochrome bitmaps for each icon
const unsigned char iconCalibration[] = { /* Your Calibration icon bitmap data */ };
const unsigned char iconSample1[] = { /* Your Sample icon bitmap data */ };
const unsigned char iconSample2[] = { /* Your Sample icon bitmap data */ };
const unsigned char iconAbout[] = { /* Your About icon bitmap data */ };

const unsigned char *icons[numOptions] = {iconCalibration, iconSample1, iconSample2, iconAbout};

int selectedOption = 0;  // Index of the currently highlighted option
unsigned long buttonPressStart = 0;
bool buttonHeld = false;
bool onMainMenu = true;
bool onCalibrate = false;
bool onCalibrate2 = false;
bool measureMode = false;

int counter = 0; 
int aState;
int aLastState;

void setup() {
   Serial.begin(9600); // Initialize serial for debugging
  tsl.setGain(TSL2561_GAIN_16X);      // No gain (1x)
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS); // 402ms integration time
  tsl2.setGain(TSL2561_GAIN_16X);      // No gain (1x)
  tsl2.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS); // 402ms integration time
   pinMode(4, OUTPUT);
   digitalWrite(4, HIGH);
   pinMode(BUTTON_PIN, INPUT_PULLUP);
   pinMode(ledPin, OUTPUT);  // Set the LED pin as output
   pinMode(ledPin2, OUTPUT);  // Set the LED pin as output
   pinMode(outputA, INPUT);
   pinMode(outputB, INPUT);
   aLastState = digitalRead(outputA);   
   tft.init();
   tft.setRotation(1);  // Set horizontal orientation
   tft.fillScreen(TFT_WHITE);  // Clear the screen with white
   drawMenu();  // Draw the initial menu
}


void loop() {
    blankValue = abs(-log(I0_blank/I0_empty));
    sensorValue = abs(-log(I_sample/I0_blank) - blankValue);
    button.loop();
    Serial.println("Isi array X:");
    for (int i = 0; i < 12; i++) {
        Serial.print("X[");
        Serial.print(i);
        Serial.print("] = ");
        Serial.println(X[i]);
    }
    // Handle encoder
    aState = digitalRead(outputA);
    if (aState != aLastState) {     
        if (digitalRead(outputB) != aState) { 
            counter++;
        } else {
            counter--;
        }

        selectedOption = (counter % currentNumOptions + currentNumOptions) % currentNumOptions;  // Ensure positive index

        if (onMainMenu) {
            drawMenu();  // Update the menu only when the position changes
        } else if (onCalibrate) {
            Kalibrasi();  // Update calibration menu
        } else if (onCalibrate2) {
            Kalibrasi2();
        }
    }
    aLastState = aState;



    // Menu logic
    if (selectedOption == 0 && button.getState() == 0 && onMainMenu) {
        currentNumOptions = 3;
        onMainMenu = false;

        onCalibrate = true;
        onCalibrate2 = false;
                measureMode = false; // Reset measure mode
        Kalibrasi();
    }

    if (selectedOption == 1 && button.getState() == 0 && onMainMenu) {
        currentNumOptions = 3;
        onMainMenu = false;
        onCalibrate = false;
        onCalibrate2 = true;
                measureMode = false; // Reset measure mode
        Kalibrasi2();
    }

    if (selectedOption == 2 && button.getState() == 0 && !onMainMenu) {
        onMainMenu = true;
        selectedOption = 0;
        currentNumOptions = 4;
        onMainMenu = true;
        onCalibrate = false;
        onCalibrate2 = false;
                measureMode = false; // Reset measure mode
        drawMenu();
    }

    // Sensor readings
    if (selectedOption == 0 && button.getState() == 0 && onCalibrate) { // Measure I0_empty
        digitalWrite(ledPin, HIGH);  // Turn on the LED
        delay(750);
      uint16_t broadband, ir;
      tsl.getLuminosity(&broadband, &ir);
      uint16_t visibleLight = broadband - ir;
      uint32_t lux = calculateLux(broadband, ir);
        I0_empty = lux; 
        Serial.println(I0_empty);
        digitalWrite(ledPin, LOW);  // Turn off the LED
        measureMode = false; // Reset measure mode
    } 

    if (selectedOption == 1 && button.getState() == 0 && onCalibrate) { // Measure I0_blank
        digitalWrite(ledPin, HIGH);  // Turn on the LED
        delay(750);
      uint16_t broadband, ir;
      tsl.getLuminosity(&broadband, &ir);
      uint16_t visibleLight = broadband - ir;
      uint32_t lux = calculateLux(broadband, ir);
        I0_blank = lux; 
        digitalWrite(ledPin, LOW);  // Turn off the LED
        measureMode = false; // Reset measure mode
    } 

    if (selectedOption == 0 && button.getState() == 0 && onCalibrate2) { // Measure I0_empty
        digitalWrite(ledPin2, HIGH);  // Turn on the LED
        delay(750);
        sensors_event_t event;
        tsl2.getEvent(&event);
        I0_empty = event.light; 
        digitalWrite(ledPin2, LOW);  // Turn off the LED
        measureMode = false; // Reset measure mode
    } 

    if (selectedOption == 1 && button.getState() == 0 && onCalibrate2) { // Measure I0_blank
        digitalWrite(ledPin2, HIGH);  // Turn on the LED
        delay(750);
        sensors_event_t event;
        tsl2.getEvent(&event);
        I0_blank = event.light; 
        digitalWrite(ledPin2, LOW);  // Turn off the LED
        measureMode = false; // Reset measure mode
    } 

    // Activate measure mode only when in the main menu and option is selected
    if (selectedOption == 2 && button.getState() == 0 && onMainMenu) { 
        measureMode = true;
    } else if (selectedOption == 3 && button.getState() == 0 && onMainMenu) { 
        measureMode = true;
    }

    // Measure when in measure mode


if (measureMode) {
    if (selectedOption == 2) { // Measure I_sample
        while (measurementIndex < 12) {
            digitalWrite(ledPin, HIGH);  // Turn on the LED
            delay(750);
            uint16_t broadband, ir;
            tsl.getLuminosity(&broadband, &ir);
            uint16_t visibleLight = broadband - ir;
            uint32_t lux = calculateLux(broadband, ir);
            X[measurementIndex] = lux; // Simpan hasil pengukuran ke array X
            digitalWrite(ledPin, LOW);  // Turn off the LED
            measurementIndex++;
            delay(60000); // Delay 1 menit sebelum pengukuran berikutnya
        }
        measureMode = false; // Reset measure mode setelah selesai 12 pengukuran
    } else if (selectedOption == 3) { // Measure I_sample for Sensor 2
        digitalWrite(ledPin2, HIGH);  // Turn on the LED
        delay(750);
        uint16_t broadband, ir;
        tsl2.getLuminosity(&broadband, &ir);
        uint16_t visibleLight = broadband - ir;
        uint32_t lux = calculateLux(broadband, ir);
        I_sample = lux; 
        digitalWrite(ledPin2, LOW);  // Turn off the LED
        measureMode = false; // Reset measure mode
    }
}

    // Serial.print("I0_empty: ");
    // Serial.println(I0_empty);
    // Serial.print("I0_blank: ");
    // Serial.println(I0_blank);
    // Serial.print("I_sample: ");
    // Serial.println(I_sample);
    // Serial.print("blankValue: ");
    // Serial.println(blankValue);
    // Serial.print("sensorValue: ");
    // Serial.println(sensorValue);

}


// Draw the menu on the TFT display
void drawMenu() {
  tft.fillScreen(TFT_WHITE);  // Clear the screen with white

  // Draw the date and time at the top
  tft.setTextColor(TFT_BLACK);
  tft.drawString("Sel, 4 Jun 24", 10, 10, 2);
  tft.drawString("05.20", 270, 10, 2);

  // Draw the menu title
  tft.setTextColor(TFT_BLACK);
  tft.drawString("A = " + String(sensorValue, 3), 110, 60, 4);  // Update sensor value

  int startIdx = 0;
  if (selectedOption == 4) {
    startIdx = 1;  // Start from "Kalibrasi Blanko" when the fourth option is selected
  }

  // Draw each menu option
  for (int i = 0; i < numOptions; i++) {
    int displayIdx = (startIdx + i) % numOptions;

    if (displayIdx == selectedOption) {
      tft.fillRect(0, 100 + i * 40, 320, 40, TFT_BLUE);  // Highlight the selected option with a full horizontal blue rectangle
      tft.setTextColor(TFT_WHITE, TFT_BLUE);  // Highlighted text color: white on blue background
    } else {
      tft.fillRect(0, 100 + i * 40, 320, 40, TFT_WHITE);  // Clear the background of unselected options
      tft.setTextColor(TFT_BLACK, TFT_WHITE);  // Unselected options: black text on white background
    }

    // Draw the icon
    tft.drawBitmap(10, 105 + i * 40, icons[displayIdx], 30, 30, (displayIdx == selectedOption) ? TFT_WHITE : TFT_BLACK);

    // Draw the text
    tft.drawString(options[displayIdx], 50, 110 + i * 40, 2);  // Draw each option, spaced vertically
  }
}

void Kalibrasi() {
  tft.fillScreen(TFT_WHITE);  // Clear the screen with white
  tft.setTextColor(TFT_BLACK);
  tft.drawString("Sel, 4 Jun 24", 10, 10, 2);
  tft.drawString("05.20", 270, 10, 2);
  tft.setTextColor(TFT_BLACK);
  tft.drawString("A = " + String(blankValue, 3), 110, 60, 4);  // Update sensor value
  for (int i = 0; i < 3; i++) {
    int displayIdx = i % 3;

    if (displayIdx == selectedOption) {
      tft.fillRect(0, 100 + i * 40, 320, 40, TFT_BLUE);  // Highlight the selected option with a full horizontal blue rectangle
      tft.setTextColor(TFT_WHITE, TFT_BLUE);  // Highlighted text color: white on blue background
    } else {
      tft.fillRect(0, 100 + i * 40, 320, 40, TFT_WHITE);  // Clear the background of unselected options
      tft.setTextColor(TFT_BLACK, TFT_WHITE);  // Unselected options: black text on white background
    }

    // Draw the text
    tft.drawString(kalibrate[displayIdx], 50, 110 + i * 40, 2);  // Draw each option, spaced vertically
  }
  if (selectedOption == 2 && button.getState() == 0){
    currentNumOptions = 4;
    onMainMenu = true;
    onCalibrate = false;
    drawMenu();
  }
}

void Kalibrasi2() {
  tft.fillScreen(TFT_WHITE);  // Clear the screen with white
  tft.setTextColor(TFT_BLACK);
  tft.drawString("Sel, 4 Jun 24", 10, 10, 2);
  tft.drawString("05.20", 270, 10, 2);
  tft.setTextColor(TFT_BLACK);
  tft.drawString("A = " + String(blankValue, 3), 110, 60, 4);  // Update sensor value
  for (int i = 0; i < 3; i++) {
    int displayIdx = i % 3;

    if (displayIdx == selectedOption) {
      tft.fillRect(0, 100 + i * 40, 320, 40, TFT_BLUE);  // Highlight the selected option with a full horizontal blue rectangle
      tft.setTextColor(TFT_WHITE, TFT_BLUE);  // Highlighted text color: white on blue background
    } else {
      tft.fillRect(0, 100 + i * 40, 320, 40, TFT_WHITE);  // Clear the background of unselected options
      tft.setTextColor(TFT_BLACK, TFT_WHITE);  // Unselected options: black text on white background
    }

    // Draw the text
    tft.drawString(kalibrate[displayIdx], 50, 110 + i * 40, 2);  // Draw each option, spaced vertically
  }
  if (selectedOption == 2 && button.getState() == 0){
    currentNumOptions = 4;
    onMainMenu = true;
    onCalibrate2 = false;
    drawMenu();
  }
}



void displayTentang() {
  tft.fillScreen(TFT_WHITE);  // Clear the screen with white
  tft.setTextColor(TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("Tentang", 160, 50, 4);
  tft.drawString("Dummy content for About", 160, 100, 2);
}

uint32_t calculateLux(uint16_t broadband, uint16_t ir) {
  unsigned long chScale;
  unsigned long channel1;
  unsigned long channel0;
  
  // Choose scale based on integration time and gain
  chScale = (1 << TSL2561_LUX_CHSCALE); // Assume no scaling for simplicity
  
  // Scale the channel values
  channel0 = (broadband * chScale) >> TSL2561_LUX_CHSCALE;
  channel1 = (ir * chScale) >> TSL2561_LUX_CHSCALE;
  
  // Find the ratio of the channel values (Channel1/Channel0)
  unsigned long ratio1 = 0;
  if (channel0 != 0) ratio1 = (channel1 << (TSL2561_LUX_RATIOSCALE + 1)) / channel0;
  unsigned long ratio = (ratio1 + 1) >> 1;
  
  unsigned int b, m;
  if (ratio <= TSL2561_LUX_K1T) { b = TSL2561_LUX_B1T; m = TSL2561_LUX_M1T; }
  else if (ratio <= TSL2561_LUX_K2T) { b = TSL2561_LUX_B2T; m = TSL2561_LUX_M2T; }
  else if (ratio <= TSL2561_LUX_K3T) { b = TSL2561_LUX_B3T; m = TSL2561_LUX_M3T; }
  else if (ratio <= TSL2561_LUX_K4T) { b = TSL2561_LUX_B4T; m = TSL2561_LUX_M4T; }
  else if (ratio <= TSL2561_LUX_K5T) { b = TSL2561_LUX_B5T; m = TSL2561_LUX_M5T; }
  else if (ratio <= TSL2561_LUX_K6T) { b = TSL2561_LUX_B6T; m = TSL2561_LUX_M6T; }
  else if (ratio <= TSL2561_LUX_K7T) { b = TSL2561_LUX_B7T; m = TSL2561_LUX_M7T; }
  else { b = TSL2561_LUX_B8T; m = TSL2561_LUX_M8T; }

  unsigned long temp = ((channel0 * b) - (channel1 * m));
  if (temp < 0) temp = 0;
  temp += (1 << (TSL2561_LUX_LUXSCALE - 1));
  
  uint32_t lux = temp >> TSL2561_LUX_LUXSCALE;
  return lux;
}
