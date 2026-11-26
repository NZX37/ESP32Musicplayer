#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <AudioFileSourceSD.h>
#include <AudioGeneratorMP3.h>
#include <AudioOutputI2S.h>

constexpr int PIN_SPI_SCK  = 12;
constexpr int PIN_SPI_MOSI = 11;
constexpr int PIN_TFT_CS   = 38;
constexpr int PIN_TFT_DC   = 9;
constexpr int PIN_TFT_RST  = 2;
constexpr int PIN_TFT_BL   = 21;

constexpr int PIN_SD_CS    = 10;
constexpr int PIN_SD_MISO  = 13;
constexpr int PIN_SD_MOSI  = PIN_SPI_MOSI;
constexpr int PIN_SD_SCK   = PIN_SPI_SCK;

constexpr int PIN_I2S_BCLK = 41;
constexpr int PIN_I2S_LRCK = 40;
constexpr int PIN_I2S_DOUT = 39;

constexpr uint16_t SCREEN_WIDTH = 240;
constexpr uint16_t SCREEN_HEIGHT = 320;
constexpr char MUSIC_DIR[] = "/music";

Adafruit_ST7789 display(PIN_TFT_CS, PIN_TFT_DC, PIN_TFT_RST);
AudioGeneratorMP3 *mp3 = nullptr;
AudioFileSourceSD *file = nullptr;
AudioOutputI2S *audioOut = nullptr;
File musicDir;
char currentTrack[96] = "";
bool cardReady = false;

static void showMessage(const char *title, const char *detail) {
  display.fillScreen(ST77XX_BLACK);
  display.setTextWrap(true);
  display.setTextSize(2);
  display.setTextColor(ST77XX_CYAN);
  display.setCursor(8, 12);
  display.println(title);
  display.setTextSize(1);
  display.setTextColor(ST77XX_WHITE);
  display.setCursor(8, 48);
  display.println(detail);
}

static bool isMp3(const char *name) {
  const size_t n = strlen(name);
  if (n < 4) return false;
  return strcasecmp(name + n - 4, ".mp3") == 0;
}

static void drawTrack(const char *name) {
  display.fillScreen(ST77XX_BLACK);
  display.setTextColor(ST77XX_GREEN);
  display.setTextSize(2);
  display.setCursor(8, 12);
  display.println("Mini Player");
  display.drawFastHLine(8, 40, SCREEN_WIDTH - 16, ST77XX_BLUE);
  display.setTextColor(ST77XX_WHITE);
  display.setTextSize(1);
  display.setCursor(8, 54);
  display.println("Now playing:");
  display.setCursor(8, 70);
  display.println(name);
  display.setTextColor(ST77XX_YELLOW);
  display.setCursor(8, 110);
  display.println("MP3 from /music");
}

static void stopPlayback() {
  if (mp3) {
    mp3->stop();
    delete mp3;
    mp3 = nullptr;
  }
  if (file) {
    delete file;
    file = nullptr;
  }
}

static bool startNextTrack() {
  if (!musicDir) musicDir = SD.open(MUSIC_DIR);
  if (!musicDir) {
    showMessage("No /music", "Create /music on the card and copy MP3 files there.");
    return false;
  }

  File entry;
  while ((entry = musicDir.openNextFile())) {
    if (entry.isDirectory() || !isMp3(entry.name())) {
      entry.close();
      continue;
    }

    const char *leaf = strrchr(entry.name(), '/');
    leaf = leaf ? leaf + 1 : entry.name();
    strncpy(currentTrack, leaf, sizeof(currentTrack) - 1);
    currentTrack[sizeof(currentTrack) - 1] = '\0';
    entry.close();

    char path[128];
    snprintf(path, sizeof(path), "%s/%s", MUSIC_DIR, currentTrack);
    stopPlayback();
    file = new AudioFileSourceSD(path);
    if (!file || !file->isOpen()) {
      stopPlayback();
      continue;
    }
    mp3 = new AudioGeneratorMP3();
    if (!mp3 || !mp3->begin(file, audioOut)) {
      stopPlayback();
      continue;
    }
    drawTrack(currentTrack);
    Serial.printf("Playing %s\n", path);
    return true;
  }

  musicDir.close();
  showMessage("Done", "Reset to play again");
  return false;
}

void setup() {
  Serial.begin(115200);
  delay(250);
  pinMode(PIN_TFT_BL, OUTPUT);
  digitalWrite(PIN_TFT_BL, HIGH);

  SPI.begin(PIN_SPI_SCK, PIN_SD_MISO, PIN_SPI_MOSI);
  display.init(SCREEN_HEIGHT, SCREEN_WIDTH);
  display.setRotation(1);
  showMessage("Mini Player", "Starting...");

  if (!SD.begin(PIN_SD_CS, SPI, 20000000)) {
    showMessage("SD error", "Check card, format (FAT32), and GPIO map.");
    Serial.println("SD initialization failed");
    return;
  }
  cardReady = true;
  Serial.println("SD initialized");
  if (!SD.exists(MUSIC_DIR)) SD.mkdir(MUSIC_DIR);

  audioOut = new AudioOutputI2S();
  audioOut->SetPinout(PIN_I2S_BCLK, PIN_I2S_LRCK, PIN_I2S_DOUT);
  audioOut->SetGain(0.15);

  showMessage("Card ready", "Searching /music for MP3 files...");
  if (!startNextTrack()) {
    showMessage("No MP3 found", "Put MP3 files in the card's /music folder.");
  }
}

void loop() {
  if (!cardReady || !mp3) {
    delay(100);
    return;
  }
  if (!mp3->loop()) {
    stopPlayback();
    startNextTrack();
  }
  delay(1);
}
