/**
   Sensor Receiver Signal Tester

   https://github.com/Andy4495/wireless-signal-tester

   Used to test Sensor transmitters.
   - Displays RSSI, LQI, Temp, Vcc, Channel # on external OLED
   - RX Channel selection:
          CHANNEL_1 - Default channel
          CHANNEL_3 - Pond sensor (Press button or Ground Pin 5 during reset)
          CHANNEL_4 - Weather station (Ground Pin 6 during reset)
   - BoosterPack Pin Definitions
    1: 3V3                      20: GND
    2: GDO2 (CC110L)            19: GDO0 (CC110L)
    3: >>RXD                    18: CS (CC100L)
    4: <<TXD                    17: -
    5: CHANNEL_3 select         16: -
    6: CHANNEL_4 select         15: MISO (CC110L)
    7: SCK                      14: MOSI (CC110L)
    8:                          13: SCLK (OLED)
    9: SCL (Fuel Tank II)       12: SDIN (OLED)
   10: SDA (Fuel Tank II)       11: CS (OLED)

   Note that on a G2 LaunchPad, pin 5 (P1.3) has a button tied to it.

   --------------------------------------------------------

   OLED Display Format:
     c,n:Rx:-sss,Q:qq
     T=tttt, V=vvvv S

   Where:
   c = Rx Channel Number
   n = Device ID
   sss = RSSI
   qq = LQI
   tttt = Sensor temperature in tenth degrees F
   vvvv = Sensor battery voltage in mV
   S = Status symbol in lower right corner of display:
       Alternates between top, middle, bottom bar every second
       to indicate that code is running and receiver is on.

   --------------------------------------------------------

   1.0 - 02/02/2019 - A.T. - Original
   1.1 - 02/09/2019 - A.T. - Update status symbol to minimize flashing
   1.2 - 02/11/2019 - A.T. - Use write commands from updated NewhavenOLED library
   1.2.1 - 02/12/2019 - A.T. - Add URL, minor updates to comments.

   --------------------------------------------------------
**/

#include "NewhavenOLED.h"
const byte OLED_ROWS = 2;                 // Number of display rows
const byte OLED_COLS = 16;             // Number of display columns
const byte CS_PIN = 11;
const byte RES_PIN = NO_PIN;          // Hardwire RESET pin pulled HIGH externally
const byte SCLK_PIN = 13;
const byte SDIN_PIN = 12;
const byte row_address[2] = {0x80, 0xC0};   // DDRAM addresses for rows (2-row models)
NewhavenOLED oled(OLED_ROWS, OLED_COLS, SDIN_PIN, SCLK_PIN, CS_PIN, RES_PIN);
char oled_text[OLED_ROWS*OLED_COLS + 1] =
{ " Signal Tester   Channel_2      "
};

#include <SPI.h>
#include <AIR430BoostFCC.h>

#define CC110L_CS  18
#define RF_GDO0    19

#define ADDRESS_REMOTE   0x01
#define ADDRESS_WEATHER  0x02
#define ADDRESS_G2       0x03    // Slim's temp
#define ADDRESS_SENSOR4  0x04    // Workshop
#define ADDRESS_SENSOR5  0x05    // Gargoyle
#define ADDRESS_REPEATER 0x06
#define ADDRESS_POND     0x07


channel_t rxChannel = CHANNEL_1;        // Can be changed by grounding pins 5 or 6 (see above)
int ch = 1;  // Integer representation of channel number

enum {WEATHER_STRUCT, TEMP_STRUCT};

struct WeatherData {
  int             BME280_T;  // Tenth degrees F
  unsigned int    BME280_P;  // Pressure in inches of Hg * 100
  int             BME280_H;  // % Relative Humidity
  int             TMP107_Ti; // Tenth degrees F
  int             TMP107_Te; // Tenth degrees F
  unsigned long   LUX;       // Lux units
  int             MSP_T;     // Tenth degrees F
  unsigned int    Batt_mV;   // milliVolts
  unsigned int    Loops;
  unsigned long   Millis;
};

struct TempSensor {
  int             MSP_T;     // Tenth degrees F
  unsigned int    Batt_mV;   // milliVolts
  unsigned int    Loops;
  unsigned long   Millis;
  unsigned int    Light_Sensor;
  unsigned int    Door_Sensor;
};

struct PondData {
  int             MSP_T;          // Tenth degrees F
  int             Submerged_T;    // Tenth degrees F
  unsigned int    Batt_mV;        // milliVolts
  int             Pump_Status;    // Unimplemented
  int             Aerator_Status; // Unimplemented
  unsigned long   Millis;
  int             Battery_T;      // Tenth degrees F
};

struct sPacket
{
  uint8_t from;              // Local node address that message originated from
  uint8_t struct_type;       // Flag to indicate type of message structure
  union {
    uint8_t message[58];     // Local node message keep even word boundary
    WeatherData weatherdata;
    TempSensor  sensordata;
    PondData    ponddata;
  };
};

struct sPacket rxPacket;

int statusFlag = 0;
char statusChar;

void setup() {

  pinMode(5, INPUT_PULLUP);   // Used to select CHANNEL_3
  pinMode(6, INPUT_PULLUP);   // Used to select CHANNEL_4

  if (digitalRead(5) == LOW) {
    rxChannel = CHANNEL_3;
    ch = 3;
    oled_text[25] = '3';
  }
  if (digitalRead(6) == LOW) {
    rxChannel = CHANNEL_4;
    ch = 4;
    oled_text[25] = '4';
  }

  oled.begin();
  oled.write(oled_text);        // Print the welcome message

  // Setup CC110L data structure
  rxPacket.from = 0;         // "from" and "struct_type" filled in by received message
  rxPacket.struct_type = 0;  // Zero them out here just for completeness
  memset(rxPacket.message, 0, sizeof(rxPacket.message));

  SPI.begin(CC110L_CS);
  delay(1000);   // Pause for the status messages and displays
  Radio.begin(ADDRESS_REMOTE, rxChannel, POWER_MAX);
}

void loop() {
  int packetSize, rxRSSI, rxLQI, crcFailed, rxT, rxmV;

  char temp_text[17];

  packetSize = Radio.receiverOn((unsigned char*)&rxPacket, sizeof(rxPacket), 1000);

  // Alternate special character symbols in lower right of OLED to indicate receiver is active.
  switch (statusFlag % 4) {
    case 0:
      statusChar = 0xFF;    // Top line
      break;
    case 1:
      statusChar = 0xB0;    // Middle line
      break;
    case 2:
      statusChar = 0x5F;    // Bottom line
      break;
    case 3:
      statusChar = 0xB0;    // Middle line
      break;
    default:
      break;
  }
  statusFlag++;

  if (packetSize > 0) {
    rxRSSI = Radio.getRssi();
    rxLQI = Radio.getLqi();
    if (Radio.getCrcBit() == 0) crcFailed = 1;
    else  crcFailed = 0;

    if (crcFailed == 0) {
      switch (rxPacket.from) {
        case (ADDRESS_WEATHER):
          rxT =  rxPacket.weatherdata.TMP107_Ti;
          rxmV = rxPacket.weatherdata.Batt_mV;
          break;

        case (ADDRESS_G2):
        case (ADDRESS_SENSOR4):
        case (ADDRESS_SENSOR5):
        case (ADDRESS_REPEATER):
          rxT =  rxPacket.sensordata.MSP_T;
          rxmV = rxPacket.sensordata.Batt_mV;
          break;

        case (ADDRESS_POND):
          rxT =  rxPacket.ponddata.Submerged_T;
          rxmV = rxPacket.ponddata.Batt_mV;
          break;
      }
    }

    // Format the display
    if (rxRSSI < -999) rxRSSI = -999;
    if (rxRSSI > 0) rxRSSI = 0;
    if (rxLQI > 99) rxLQI = 99;
    if (rxLQI < 0) rxLQI = 0;
    snprintf(oled_text, OLED_COLS + 1, "%d,%d:Rs:-%3d,Q:%2d", ch, rxPacket.from, -rxRSSI, rxLQI);
    if (crcFailed == 1) {
      strncat(oled_text, "CRC Failed!     ", OLED_COLS + 1);
    }
    else { // CRC was valid
      if (rxT > 9999) rxT = 9999;
      if (rxT < -999) rxT = -999;
      if (rxmV > 9999) rxmV = 9999;
      if (rxmV < -999) rxmV = -999;
      snprintf(temp_text, OLED_COLS + 1, "T:%4d, V:%4d   ", rxT, rxmV);
      strncat(oled_text, temp_text, OLED_COLS + 1);
    }

    oled_text[31] = statusChar;
    oled.write(oled_text);
  }

  else { // Just update the status symbol without flashing all the chars on the display
    oled.write(15, 1, statusChar);
  }
}
