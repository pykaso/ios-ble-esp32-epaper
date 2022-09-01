#define  ENABLE_GxEPD2_display 0

// BLE
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

// ePAPER
#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <U8g2_for_Adafruit_GFX.h>
#include <SPI.h>
#define SCREEN_WIDTH  400.0
#define SCREEN_HEIGHT 300.0

enum alignment {LEFT, RIGHT, CENTER};

// Connections for e.g. Waveshare ESP32 e-Paper Driver Board
static const uint8_t EPD_BUSY = 25;
static const uint8_t EPD_CS   = 15;
static const uint8_t EPD_RST  = 26; 
static const uint8_t EPD_DC   = 27; 
static const uint8_t EPD_SCK  = 13;
static const uint8_t EPD_MISO = 12; // Master-In Slave-Out not used, as no data from display
static const uint8_t EPD_MOSI = 14;

GxEPD2_3C<GxEPD2_420c, GxEPD2_420c::HEIGHT> display(GxEPD2_420c(/*CS=D8*/ EPD_CS, /*DC=D3*/ EPD_DC, /*RST=D4*/ EPD_RST, /*BUSY=D2*/ EPD_BUSY));
U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;  // Select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall


char Message[] = "Hello World!";
const unsigned char Bitmap_test[] PROGMEM = { 0xff, 0xff, 0xfd, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xf8, 0xff, 0x8f, 0xfd, 0xff, 0x8f, 0xf8, 0xff, 0x87, 0xff, 0xff, 0x0f, 0xf8, 0xff, 0xc3, 0xff, 0xfe, 0x1f, 0xf8, 0xff, 0xe3, 0xff, 0xfe, 0x3f, 0xf8, 0xff, 0xf7, 0xe0, 0x3f, 0x7f, 0xf8, 0xff, 0xff, 0x80, 0x0f, 0xff, 0xf8, 0xff, 0xff, 0x07, 0x07, 0xff, 0xf8, 0xff, 0xfe, 0x1f, 0xc3, 0xff, 0xf8, 0xff, 0xfc, 0x3f, 0xe1, 0xff, 0xf8, 0xff, 0xfc, 0x7f, 0xf1, 0xff, 0xf8, 0xff, 0xfc, 0xff, 0xf9, 0xff, 0xf8, 0xff, 0xf8, 0xff, 0xf8, 0xff, 0xf8, 0xf0, 0x38, 0xff, 0xf8, 0xe0, 0x78, 0xf0, 0x38, 0xff, 0xf8, 0xe0, 0x78, 0xff, 0xf8, 0xff, 0xf8, 0xff, 0xf8, 0xff, 0xfc, 0xff, 0xf8, 0xff, 0xf8, 0xff, 0xfc, 0x7f, 0xf1, 0xff, 0xf8, 0xff, 0xfc, 0x7f, 0xf1, 0xff, 0xf8, 0xff, 0xfe, 0x1f, 0xe3, 0xff, 0xf8, 0xff, 0xff, 0x0f, 0x87, 0xff, 0xf8, 0xff, 0xff, 0x80, 0x0f, 0xff, 0xf8, 0xff, 0xff, 0xe0, 0x3f, 0xff, 0xf8, 0xff, 0xe3, 0xff, 0xfe, 0x3f, 0xf8, 0xff, 0xc3, 0xff, 0xfe, 0x1f, 0xf8, 0xff, 0x87, 0xff, 0xff, 0x0f, 0xf8, 0xff, 0x8f, 0xff, 0xff, 0x8f, 0xf8, 0xff, 0x9f, 0xf8, 0xff, 0xcf, 0xf8, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xfd, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8 };

std::vector<unsigned char> to_vector(std::string const& str)
{
    // don't forget the trailing 0...
    return std::vector<unsigned char>(str.data(), str.data() + str.length() + 1);
}


void drawImage() {
  display.firstPage();
  do
  {
    display.drawInvertedBitmap(0, 0, Bitmap_test, 45, 45, GxEPD_BLACK);
  }
  while (display.nextPage());
}

void drawPixels(std::string data) {
    uint32_t pixel = 0; 
    display.setFullWindow();
    display.firstPage();
    display.writeScreenBuffer();
    
    for (int i=0; i<data.length(); i++){
        int color = GxEPD_WHITE;
        switch (data[i]){
          case 0:
            color = GxEPD_WHITE;
            break;
          case 1:
            color = GxEPD_BLACK;
            break;
          case 2:
            color = GxEPD_RED;
            break;
        }
        int x = pixel % 163;//display.width();
        int y = pixel / 163;//display.width();

       /* // Switch to next page in half of the screen
        if(x == 0 && y == 240)
        {
              Serial.println("Next page");
              display.nextPage();
        }*/

        display.drawPixel(x, y, color);
        //Serial.printf("[%d,%d,%d],", x, y, color);
        pixel++;
      }
      display.nextPage();
}

class MessageCallbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *characteristic)
    {
        std::string data = characteristic->getValue();
        std::string CTRL = "ABC";
        const char* sdata = data.c_str();
        Serial.println(data.c_str());

        if (data[0] == CTRL[0]) {
          Serial.println("A - Text");
          char *copy = strdup(sdata);
          copy = copy + 1;
          Serial.println(copy);
          drawString(copy);
        } else if (data[0] == CTRL[1]) {
          Serial.println("B - Icon");
          drawImage();
        } else {
          Serial.println("C - image");
          Serial.println("data length: ");
          Serial.println(data.length());
          drawPixels(data);
          // auto image = to_vector(data);
          // unsigned char image2[data.length() + 1];
          // std::copy(data.data(), data.data() + data.length() + 1, image2);


          // display.firstPage();
          // do
          // {
          //   display.drawInvertedBitmap(0, 0, image2, 200, 200, GxEPD_BLACK);
          // }
          // while (display.nextPage());
        }
        
        // if (strcmp(data.c_str(), foo) == 0) {
        //   drawImage();
        // } else {
        
        //   u8g2Fonts.setFont(u8g2_font_helvB12_tf);
        //   display.setTextColor(GxEPD_BLACK);
        //   int16_t tbx, tby; uint16_t tbw, tbh;
        //   display.getTextBounds(data.c_str(), 0, 0, &tbx, &tby, &tbw, &tbh);
        //   uint16_t x = (display.width() - tbw) / 2;
        //   uint16_t y = (display.height() + tbh) / 2; // y is base line!
        //   display.setFullWindow();
        //   display.firstPage();
        //   do
        //   {
        //     display.fillScreen(GxEPD_WHITE);
        //     display.setCursor(x, y);
        //     display.print(data.c_str());
        //   }
        //   while (display.nextPage());
        // }

        //u8g2Fonts.setFont(u8g2_font_helvB08_tf);
        //drawString(SCREEN_WIDTH / 2, 0, data.c_str(), CENTER);
    }

    void onRead(BLECharacteristic *characteristic)
    {
        characteristic->setValue("Foobar");
    }
};

void setup() {
  Wire.begin();
  Serial.begin(115200);

#ifdef BUILTIN_LED
  pinMode(BUILTIN_LED, INPUT); // If it's On, turn it off and some boards use GPIO-5 for SPI-SS, which remains low after screen use
  digitalWrite(BUILTIN_LED, HIGH);
#endif

  // ePaper
  InitialiseDisplay();

  Serial.println("Starting BLE work!");

  BLEDevice::init("Long name works now");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setValue("Hello World says Neil");
  pCharacteristic->setCallbacks(new MessageCallbacks());
  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");

  delay(1000);
  Serial.println("ePaper demo");
  helloWorld();
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);
}



void InitialiseDisplay() {
  Serial.println("InitialiseDisplay()");
  display.init(0);
  SPI.end();
  SPI.begin(EPD_SCK, EPD_MISO, EPD_MOSI, EPD_CS);
  u8g2Fonts.begin(display); // connect u8g2 procedures to Adafruit GFX
  u8g2Fonts.setFontMode(1);                  // use u8g2 transparent mode (this is default)
  u8g2Fonts.setFontDirection(0);             // left to right (this is default)
  u8g2Fonts.setForegroundColor(GxEPD_BLACK); // apply Adafruit GFX color
  u8g2Fonts.setBackgroundColor(GxEPD_WHITE); // apply Adafruit GFX color
  u8g2Fonts.setFont(u8g2_font_helvB10_tf);   // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
  display.fillScreen(GxEPD_WHITE);
  display.setFullWindow();
}


void helloWorld()
{
  Serial.println("helloWorld");
  display.setRotation(1);
  //display.setFont(&FreeMonoBold9pt7b);
  u8g2Fonts.setFont(u8g2_font_helvB12_tf);
  display.setTextColor(GxEPD_BLACK);
  int16_t tbx, tby; uint16_t tbw, tbh;
  display.getTextBounds(Message, 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t x = (display.width() - tbw) / 2;
  uint16_t y = (display.height() + tbh) / 2; // y is base line!
  display.setFullWindow();
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(x, y);
    display.print(Message);
  }
  while (display.nextPage());
  Serial.println("helloWorld done");
}

void drawString(const char* value)
{
  u8g2Fonts.setFont(u8g2_font_helvB12_tf);
  display.setTextColor(GxEPD_BLACK);
  int16_t tbx, tby; uint16_t tbw, tbh;
  display.getTextBounds(value, 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t x = (display.width() - tbw) / 2;
  uint16_t y = (display.height() + tbh) / 2; // y is base line!
  display.setFullWindow();
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(x, y);
    display.print(value);
  }
  while (display.nextPage());
}
