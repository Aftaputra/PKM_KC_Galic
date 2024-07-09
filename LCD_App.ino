#include <TFT_eSPI.h>  // Include the TFT_eSPI library
#define ENCODER_DO_NOT_USE_INTERRUPTS
#include <Encoder.h>

Encoder myEnc(13, 12);
TFT_eSPI tft = TFT_eSPI();  // Create an instance of the TFT_eSPI library

const int numOptions = 5;  // Number of menu options
const char *options[numOptions] = {"Sambungkan ke HP", "Kalibrasi Blanko", "Pengukuran Sampel", "Profil", "Tentang"};

// Define icons as bitmaps (monochrome bitmaps)
// These are placeholders; you need to generate your own monochrome bitmaps for each icon
const unsigned char iconBluetooth[] = { /* Your Bluetooth icon bitmap data */ };
const unsigned char iconCalibration[] = { /* Your Calibration icon bitmap data */ };
const unsigned char iconSample[] = { /* Your Sample icon bitmap data */ };
const unsigned char iconProfile[] = { /* Your Profile icon bitmap data */ };
const unsigned char iconAbout[] = { /* Your About icon bitmap data */ };

const unsigned char *icons[numOptions] = {iconBluetooth, iconCalibration, iconSample, iconProfile, iconAbout};

int selectedOption = 0;  // Index of the currently highlighted option

void setup() {
  Serial.begin(9600); // Initialize serial for debugging

  tft.init();
  tft.setRotation(1);  // Set horizontal orientation
  tft.fillScreen(TFT_WHITE);  // Clear the screen with white
  drawMenu();  // Draw the initial menu
}

long position = -999;

void loop() {
  long newPos = myEnc.read() / 2;
  if (newPos != position) {
    position = newPos;
    selectedOption = position % numOptions;
    if (selectedOption < 0) selectedOption += numOptions;
    drawMenu();  // Update the menu only when the position changes
  }
  Serial.println(selectedOption);
}

void drawMenu() {
  tft.fillScreen(TFT_WHITE);  // Clear the screen with white

  // Draw the date and time at the top
  tft.setTextColor(TFT_BLACK);
  tft.drawString("Sel, 4 Jun 24", 10, 10, 2);
  tft.drawString("05.20", 270, 10, 2);

  // Draw the menu title
  tft.setTextColor(TFT_BLACK);
  tft.drawString("Menu Utama", 120, 40, 2);

  int startIdx = 0;
  if (selectedOption == 4) {
    startIdx = 1;  // Start from "Kalibrasi Blanko" when the fourth option is selected
  }

  // Draw each menu option
  for (int i = 0; i < numOptions; i++) {
    int displayIdx = (startIdx + i) % numOptions;

    if (displayIdx == selectedOption) {
      tft.fillRect(0, 70 + i * 40, 320, 40, TFT_BLUE);  // Highlight the selected option with a full horizontal blue rectangle
      tft.setTextColor(TFT_WHITE, TFT_BLUE);  // Highlighted text color: white on blue background
    } else {
      tft.fillRect(0, 70 + i * 40, 320, 40, TFT_WHITE);  // Clear the background of unselected options
      tft.setTextColor(TFT_BLACK, TFT_WHITE);  // Unselected options: black text on white background
    }

    // Draw the icon
    tft.drawBitmap(10, 75 + i * 40, icons[displayIdx], 30, 30, (displayIdx == selectedOption) ? TFT_WHITE : TFT_BLACK);

    // Draw the text
    tft.drawString(options[displayIdx], 50, 80 + i * 40, 2);  // Draw each option, spaced vertically
  }
}
