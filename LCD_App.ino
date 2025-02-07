#include <TFT_eSPI.h>  // Include the TFT_eSPI library
#define ENCODER_DO_NOT_USE_INTERRUPTS
#include <Encoder.h>
#include <esp_sleep.h>
#include <ezButton.h>
#define BUTTON_PIN 14
#define DEEPSLEEP_DURATION 4  // Time in seconds to hold the button for sleep

ezButton button(14);  // create ezButton object that attach to pin 7;

Encoder myEnc(13, 12);
TFT_eSPI tft = TFT_eSPI();  // Create an instance of the TFT_eSPI library

const int numOptions = 5;  // Number of menu options
int currentNumOptions = numOptions;  // Number of menu options
const char *options[numOptions] = {"Sambungkan ke HP", "Kalibrasi Blanko", "Pengukuran Sampel", "Profil", "Tentang"};
const int Halaman1 = 3;  // Number of menu options
const char *pilihan[Halaman1] = {""};

// Define icons as bitmaps (monochrome bitmaps)
// These are placeholders; you need to generate your own monochrome bitmaps for each icon
const unsigned char iconBluetooth[] = { /* Your Bluetooth icon bitmap data */ };
const unsigned char iconCalibration[] = { /* Your Calibration icon bitmap data */ };
const unsigned char iconSample[] = { /* Your Sample icon bitmap data */ };
const unsigned char iconProfile[] = { /* Your Profile icon bitmap data */ };
const unsigned char iconAbout[] = { /* Your About icon bitmap data */ };

const unsigned char *icons[numOptions] = {iconBluetooth, iconCalibration, iconSample, iconProfile, iconAbout};

int selectedOption = 0;  // Index of the currently highlighted option
unsigned long buttonPressStart = 0;
bool buttonHeld = false;
bool onMainMenu = true;
bool MasukHal;

void setup() {
  Serial.begin(9600); // Initialize serial for debugging
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  tft.init();
  tft.setRotation(1);  // Set horizontal orientation
  tft.fillScreen(TFT_WHITE);  // Clear the screen with white
  drawMenu();  // Draw the initial menu
}

long position = -999;

void loop() {
  button.loop();
  if(button.getState() == 0) {
    MasukHal = true;
  }
  if (onMainMenu) {
    // Handle encoder
    long newPos = myEnc.read() / 2;
    if (newPos != position) {
      position = newPos;
      selectedOption = (position % currentNumOptions + currentNumOptions) % currentNumOptions;  // Ensure positive index
      drawMenu();  // Update the menu only when the position changes
    }
    Serial.println(selectedOption);


  }
      if(selectedOption == 0 && MasukHal == true){
      currentNumOptions = 3;
      displaySambungkanKeHP();
    }
  // // Handle button press for sleep and navigation
  // if (digitalRead(BUTTON_PIN) == LOW) {
  //   if (!buttonHeld) {
  //     buttonPressStart = millis();
  //     buttonHeld = true;
  //   } else if (millis() - buttonPressStart >= DEEPSLEEP_DURATION * 1000) {
  //     // Put the ESP32 into light sleep
  //     Serial.println("Entering light sleep...");
  //     tft.fillScreen(TFT_BLACK);
  //     tft.setTextColor(TFT_WHITE, TFT_BLACK);
  //     tft.drawString("Entering light sleep...", 60, 120, 2);
  //     delay(1000);  // Show the message for 1 second before sleeping

  //     enterLightSleep();
  //   }
  // } else {
  //   if (buttonHeld) {
  //     if (onMainMenu) {
  //       // Call the appropriate function based on the selected option
  //       switch (selectedOption) {
  //         case 0:
  //           displaySambungkanKeHP();
  //           break;
  //         case 1:
  //           displayKalibrasiBlanko();
  //           break;
  //         case 2:
  //           displayPengukuranSampel();
  //           break;
  //         case 3:
  //           displayProfil();
  //           break;
  //         case 4:
  //           displayTentang();
  //           break;
  //       }
  //       onMainMenu = false;
  //     } else {
  //       drawMenu();
  //       onMainMenu = true;
  //     }
  //   }
  //   buttonHeld = false;
  // }
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



void displaySambungkanKeHP() {
    tft.drawString("Menuuu", 50, 40, 2);  // Draw each option, spaced vertically
}


void displayKalibrasiBlanko() {
  tft.fillScreen(TFT_WHITE);  // Clear the screen with white
  tft.setTextColor(TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("Kalibrasi Blanko", 160, 50, 4);
  tft.drawString("Dummy content for Calibration", 160, 100, 2);
  // Add more UI elements for this page as needed
}

void displayPengukuranSampel() {
  tft.fillScreen(TFT_WHITE);  // Clear the screen with white
  tft.setTextColor(TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("Pengukuran Sampel", 160, 50, 4);
  tft.drawString("Dummy content for Sample", 160, 100, 2);
  // Add more UI elements for this page as needed
}

void displayProfil() {
  tft.fillScreen(TFT_WHITE);  // Clear the screen with white
  tft.setTextColor(TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("Profil", 160, 50, 4);
  tft.drawString("Dummy content for Profile", 160, 100, 2);
  // Add more UI elements for this page as needed
}

void displayTentang() {
  tft.fillScreen(TFT_WHITE);  // Clear the screen with white
  tft.setTextColor(TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("Tentang", 160, 50, 4);
  tft.drawString("Dummy content for About", 160, 100, 2);
  // Add more UI elements for this page as needed
}
