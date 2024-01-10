#include <Arduino.h>
#include <BLEDevice.h>
#include <FastLED.h>
#include <M5Unified.h>
/*
how to use.

1.set serviceUUID empty, like a bellow
  static BLEUUID serviceUUID("");
2.build and run to find your service UUID
3.you will find all BLE device list, check your power source's UUID
4.set your serviceUUID and DEF_BLE_ADDR.
5.set TARGET_CHARA empty and build & run.
5.check characteristic list ofrom your power source.
  XXXXXX63-XXXXXXXX******  is power data.
6.set your TARGET_CHARA
7.set your power zone at setRGB method.

tested device
- WS2812B LED tape
- 5V 4A ac adapter
- wahoo kickr v6

*/

//LED
#define LED_PIN     5
#define NUM_LEDS    130
#define BRIGHTNESS  100
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

//inner LED
  /*
#define INNER_LED_PIN 2
#define INNER_LED_COUNT 1
CRGB inner_leds[INNER_LED_COUNT];
  */

//power zone
#define FTP 260
#define ZONE_6 FTP*1.19
#define ZONE_5 FTP*1.05
#define ZONE_4 FTP*0.9
#define ZONE_3 FTP*0.76
#define ZONE_2 FTP*0.6


#define DEF_BLE_ADDR "f2:04:ad:eb:1e:88" // my kickr 
#define TARGET_CHARA "00002a63-0000-1000-8000-00805f9b34fb" // its my kickr's id, find your charas

static BLEUUID serviceUUID("00001818-0000-1000-8000-00805f9b34fb");// its my kickr, find yours

static BLEAdvertisedDevice* myDevice;
static BLERemoteService* pRemoteService;
static BLEClient*  pClient;

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

bool  doConnect = false;                
bool  doScan = false;                   
bool  deviceConnected = false;          
bool  isRegistered = false;      
/////////////////////////////////////////////////////////////////
/////LED
/////////////////////////////////////////////////////////////////
int rgb[]= {100, 100, 100}; // RGB
int power = 0;
int powerA1 = 0;
int powerA2 = 0;
int power3sec = 0;

int lastStatus = -1;


void ledOn() {
  Serial.print("clear");
  Serial.println();
  // FastLED.clear();  // clear all pixel data
  for(int i=0; i<NUM_LEDS; i++) { // For each pixel...
    leds[i] = CRGB( rgb[0],rgb[1],rgb[2]);//RBG!!!
  }
  Serial.print("show RGB");
  Serial.println();
    FastLED.show();
}
void statusScreenOn() {
  /*
  for m5stamp, show status to other LED
  if (doConnect == false) {
    // startConnection
    inner_leds[0] = CRGB::Blue;
  }else if (deviceConnected == true) {
    if (isRegistered) {
    // Registered
      inner_leds[0] = CRGB::White;
    }else{
    // not Registered
      inner_leds[0] = CRGB::Yellow;
    }
  }else{
    // disconnect
      inner_leds[0] = CRGB::Red;
  }
  Serial.print("show status");
  Serial.println();
  FastLED.show();
  */
 int textSize = 4;
 int color = BLACK;
 int status = 0;
 String textMessage = "";
  if (doConnect == false) {
    // startConnection
    status=0;
    color = BLUE;
    textMessage = "startConnection";
  }else if (deviceConnected == true) {
    if (isRegistered) {
    // Registered
    status=1;
    color = GREEN;
    textMessage = "Registered";
    }else{
    // not Registered
    status=2;
    color = ORANGE;
    textMessage = "not Registered";
    }
  }else{
    // disconnect
    status=3;
    color = RED;
    textMessage = "disconnect";
  }
  if(status != lastStatus){
    Serial.print("status change");
    M5.Lcd.clear();
    M5.Lcd.fillScreen(color);
    M5.Lcd.setCursor(0,80);
    M5.Lcd.setTextSize(textSize);
    M5.Lcd.println(textMessage);
    M5.update();
  }else{
  Serial.print("status keep");
  }
  lastStatus=status;
  Serial.println();
}

void setRGB() {
  power3sec = (power+powerA1+powerA2)/3;
  
  Serial.print(" power color: ");
  Serial.print(power);
  Serial.print(",");
  Serial.print(powerA1);
  Serial.print(",");
  Serial.print(powerA2);
  Serial.print("::: ");
  if(power3sec > ZONE_6){
    rgb[0] = 255;
    rgb[1] = 0;
    rgb[2] = 0;
  Serial.print("red");
  }else if(power3sec > ZONE_5){
    rgb[0] = 255;
    rgb[1] = 110;
    rgb[2] = 0;
  Serial.print("orange");
  }else if(power3sec > ZONE_4){
    rgb[0] = 255;
    rgb[1] = 255;
    rgb[2] = 0;
  Serial.print("yellow");
  }else if(power3sec > ZONE_3){
    rgb[0] = 0;
    rgb[1] = 255;
    rgb[2] = 0;
  Serial.print("green");
  }else if(power3sec > ZONE_2){
    rgb[0] = 0;
    rgb[1] = 0;
    rgb[2] = 255;
  Serial.print("blue");
  } else {
    rgb[0] = 30;
    rgb[1] = 30;
    rgb[2] = 30;
  Serial.print("gray");
  }
  Serial.print(power3sec);
  Serial.println();
  powerA1 = power;
  powerA2 = powerA1;
}    

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
  //Serial.print(" power: ");
  power = pData[3]*256+pData[2];
  //Serial.print(power);
  //Serial.println();
}


bool getFromServer() {
      Serial.println("getFromServer");

      if (pRemoteService == nullptr) {
        pClient->disconnect();
        Serial.print("disconnect ");
      }
        
      Serial.print("TARGET_CHARA:");
      Serial.println(TARGET_CHARA);
  std::map<std::string, BLERemoteCharacteristic*>* mapCharacteristics = pRemoteService->getCharacteristics();
  for (std::map<std::string, BLERemoteCharacteristic*>::iterator i = mapCharacteristics->begin(); i != mapCharacteristics->end(); ++i) {

    if( TARGET_CHARA == i->first){
      Serial.println("/// found power mesurement!:");
      Serial.print(" getHandle:");
      Serial.printf("%u",i->second->getHandle());
      Serial.print(" readUInt8:");
      Serial.printf("%u",i->second->readUInt8());
      Serial.print(" readUInt16:");
      Serial.printf("%u",i->second->readUInt16());
      Serial.print(" readUInt32:");
      Serial.printf("%u",i->second->readUInt32());
      Serial.print(" readValue:");
      Serial.printf(i->second->readValue().c_str());
      Serial.print(" toString:");
      Serial.printf(i->second->toString().c_str());
      Serial.println("/// end power mesurement!:");
      if (i->second->canNotify()) {
        Serial.println(" - Add Notify");
        i->second->registerForNotify(notifyCallback);
        isRegistered = true;
      }
    }else{
      //Serial.print("other chara:");
      //Serial.println(i->first.c_str());
    }
  }
  return true;
}



/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

void connectToServer() {
      Serial.println("connectToServer");
  
        Serial.println("doConnect1");
        pClient->connect(myDevice);
        Serial.println("doConnect2");
        pRemoteService = pClient->getService(serviceUUID);
}
class funcClientCallbacks: public BLEClientCallbacks {
    void onConnect(BLEClient* pClient) {
        Serial.println("funcClientCallbacks onConnect");
        deviceConnected = true;
    };
    void onDisconnect(BLEClient* pClient) {
        Serial.println("funcClientCallbacks onDisconne");
        deviceConnected = false;
        doConnect = false;
        isRegistered = false;
        startConnection();
    };
    void startConnection() {
        Serial.println("connectToServer");
        pClient  = BLEDevice::createClient();
        pClient->setClientCallbacks(new funcClientCallbacks());
        doConnect = true;
        //end scan
        Serial.println("end scan");
        BLEDevice::getScan()->stop();
    }
};


class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      Serial.print("BLE found");
      Serial.println(advertisedDevice.toString().c_str());
      if( DEF_BLE_ADDR == advertisedDevice.getAddress().toString() ){
        // connect if my target devicce
          Serial.println("connect:");
        BLEDevice::getScan()->stop();
        myDevice = new BLEAdvertisedDevice(advertisedDevice);
        Serial.print("connect to:");
        Serial.println(myDevice->getAddress().toString().c_str());
  
        Serial.println("ble_name:");
        Serial.println(myDevice->getName().c_str());
        Serial.println("getServiceUUID:");
        Serial.println(myDevice->getServiceUUID().toString().c_str());

        //start connection
        startConnection();
      }
    };
    
    void startConnection() {
        Serial.println("connectToServer");
        pClient  = BLEDevice::createClient();
        pClient->setClientCallbacks(new funcClientCallbacks());
        doConnect = true;
        //end scan
        Serial.println("end scan");
        BLEDevice::getScan()->stop();
    }
};
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  setCpuFrequencyMhz(160);
  M5.begin();
  //LED
  delay( 3000 ); // power-up safety delay
  // FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  //FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  
  FastLED.setBrightness(BRIGHTNESS);

  //FastLED.addLeds<NEOPIXEL, INNER_LED_PIN>(inner_leds, INNER_LED_COUNT);
  FastLED.clear();
  
  Serial.begin(115200);
  Serial.println("BLE scan start");
  BLEDevice::init("");
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(10);
}
void loop() {
  //Serial.println("loop");
  statusScreenOn();    
  if (doConnect == false) {
  }else if (deviceConnected == true) {
    if (isRegistered) {
      //readFromServer();
    }else{
      getFromServer();
    }
  }else{
    connectToServer();
  }

  //LED
  setRGB();   
  ledOn();   
  delay(1000);
}