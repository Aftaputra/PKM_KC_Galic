#include <TFT_eSPI.h>  // Include the TFT_eSPI library
#define ENCODER_DO_NOT_USE_INTERRUPTS
#include <Encoder.h>
#include <esp_sleep.h>
#include <ezButton.h>
#define BUTTON_PIN 14
#define DEEPSLEEP_DURATION 4  // Time in seconds to hold the button for sleep
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>
#include <cmath>


int ledPin = 26;  // LED pin
int ledPin2 = 27;  // LED pin
Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);
Adafruit_TSL2561_Unified tsl2 = Adafruit_TSL2561_Unified(TSL2561_ADDR_HIGH, 12345);
float I0_empty = 0.0, I0_blank = 0.0, A_blank = 0.00;
float I_sample;
float A_sample;

ezButton button(14);  // create ezButton object that attach to pin 7;

Encoder myEnc(13, 12);
TFT_eSPI tft = TFT_eSPI();  // Create an instance of the TFT_eSPI library

const int numOptions = 4;  // Number of menu options
int currentNumOptions = numOptions;  // Number of menu options
const char *options[numOptions] = {"Kalibrasi 1", "Kalibrasi 2", "Sensor 1", "Sensor 2"};
const char *kalibrate[numOptions] = {"Baca Kosong", "Baca Blanko", "Kembali"};
const int Halaman1 = 3;  // Number of menu options
const char *pilihan[Halaman1] = {""};
float sensorValue;
float blankValue;
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
bool MasukHal;

void setup() {
  Serial.begin(9600); // Initialize serial for debugging
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);  // Set the LED pin as output
  pinMode(ledPin2, OUTPUT);  // Set the LED pin as output
  tft.init();
  tft.setRotation(1);  // Set horizontal orientation
  tft.fillScreen(TFT_WHITE);  // Clear the screen with white
  drawMenu();  // Draw the initial menu
}

long position = -999;

void loop() {
  blankValue = abs(-log(I0_blank/I0_empty));
  sensorValue = abs(-log(I_sample/I0_blank) - blankValue);
  button.loop();
  if(button.getState() == 0) {
    MasukHal = true;
  }
  if (onMainMenu == true) {
    // Handle encoder
    long newPos = myEnc.read() / 2;
    if (newPos != position) {
      position = newPos;
      selectedOption = (position % currentNumOptions + currentNumOptions) % currentNumOptions;  // Ensure positive index
      drawMenu();  // Update the menu only when the position changes
    }
    Serial.println(selectedOption);
    Serial.println(button.getState());
  }
    if (onCalibrate == true) {
    // Handle encoder
    currentNumOptions = 3;
    long newPos = myEnc.read() / 2;
    if (newPos != position) {
      position = newPos;
      selectedOption = (position % currentNumOptions + currentNumOptions) % currentNumOptions;  // Ensure positive index
      Kalibrasi();  // Update the menu only when the position changes
    }
    Serial.println(selectedOption);
    Serial.println(button.getState());
  }
  if(selectedOption == 0 && button.getState() == 0 && onMainMenu == true){
      currentNumOptions = 3;
      onMainMenu = false;
      onCalibrate = true;
      Kalibrasi();
    }
      if(selectedOption == 1 && button.getState() == 0 && onMainMenu == true){
      currentNumOptions = 3;
      onMainMenu = false;
      onCalibrate = true;
      Kalibrasi2();
    }
    if(selectedOption == 2 && button.getState() == 0 && onMainMenu == false){
      currentNumOptions = 4;
      onMainMenu = true;
      onCalibrate = false;
      drawMenu();
    }
    
    Serial.println(selectedOption);
    Serial.println(button.getState());
    //Pengukuran Blank 1
    if (selectedOption == 0 && button.getState() == 0 && onCalibrate == true){ // Measure I0_empty
      digitalWrite(ledPin, HIGH);  // Turn on the LED
      delay(750);
      sensors_event_t event;
      tsl.getEvent(&event);
      I0_empty = event.light; 
      digitalWrite(ledPin, LOW);  // Turn off the LED
    } 
    if (selectedOption == 1 && button.getState() == 0 && onCalibrate == true){ // Measure I0_empty
      digitalWrite(ledPin, HIGH);  // Turn on the LED
      delay(750);
      sensors_event_t event;
      tsl.getEvent(&event);
      I0_blank = event.light; 
      digitalWrite(ledPin, LOW);  // Turn off the LED
    } 
    //Pengukuran Sensor 1
      if (selectedOption == 2 && button.getState() == 0 && onMainMenu == true){ // Measure I0_empty
      digitalWrite(ledPin, HIGH);  // Turn on the LED
      delay(750);
      sensors_event_t event;
      tsl.getEvent(&event);
      I_sample = event.light; 
      digitalWrite(ledPin, LOW);  // Turn off the LED
    } 
        //Pengukuran Sensor 2
      if (selectedOption == 3 && button.getState() == 0 && onMainMenu == true){ // Measure I0_empty
      digitalWrite(ledPin, HIGH);  // Turn on the LED
      delay(750);
      sensors_event_t event;
      tsl2.getEvent(&event);
      I_sample = event.light; 
      digitalWrite(ledPin, LOW);  // Turn off the LED
    } 
}

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
    int displayIdx = i % numOptions;

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
    int displayIdx = i % numOptions;

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

void displayTentang() {
  tft.fillScreen(TFT_WHITE);  // Clear the screen with white
  tft.setTextColor(TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("Tentang", 160, 50, 4);
  tft.drawString("Dummy content for About", 160, 100, 2);
  // Add more UI elements for this page as needed
}
