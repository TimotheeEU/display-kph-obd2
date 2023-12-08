#include <Arduino.h>
#include <CAN.h>
#include <OBD2.h>
#include <FastLED.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include "config.h"

// ARRAY OF PID's VALUES
const int PIDS[] = {
    ENGINE_RPM,
    VEHICLE_SPEED,
    ENGINE_COOLANT_TEMPERATURE,
};

const int NUM_PIDS = sizeof(PIDS) / sizeof(PIDS[0]);

MCUFRIEND_kbv tft;

void startupSeq();
void attemptConn();
void failedConn();
void succesfullConn();
void valueTitleText();

void setup()
{
  uint16_t ID = tft.readID();
  Serial.begin(9600);
  tft.begin(ID);

  // STARTUP ANIMATION
  startupSeq();

  while (!Serial)
    ;

  // CONNECTION WITH CAN
  while (true)
  {
    attemptConn();

    if (!OBD2.begin())
    {
      failedConn();
    }
    else
    {
      succesfullConn();
      break;
    }
  }

  // SHOW TOP AND BOTTOM TITLE + VALUE TITLE AND SUBTEXT
  valueTitleText();
}

String floatToString(float value)
{
  char buffer[10];
  dtostrf(value, 0, 0, buffer);
  String result = String(buffer);

  int dotIndex = result.indexOf('.');
  if (dotIndex != -1)
  {
    result = result.substring(0, dotIndex);
  }
  return result;
}

float prevEngineRPM = 0.0;
float prevVehicleSpeed = -1.0;
float prevEngineCoolantTemp = 0.0;

void loop()
{
  // Float values read from the OBD2 CAN connection
  float engineRPM = OBD2.pidRead(ENGINE_RPM);
  float vehicleSpeed = OBD2.pidRead(VEHICLE_SPEED);
  float engineCoolantTemp = OBD2.pidRead(ENGINE_COOLANT_TEMPERATURE);

  // Converting all floats to String for Display
  String engineRPM_s = floatToString(engineRPM);
  String vehicleSpeed_s = floatToString(vehicleSpeed);
  String engineCoolantTemp_s = floatToString(engineCoolantTemp);

  bool engineRPMChanged = (engineRPM != prevEngineRPM);
  bool vehicleSpeedChanged = (vehicleSpeed != prevVehicleSpeed);
  bool engineCoolantTempChanged = (engineCoolantTemp != prevEngineCoolantTemp);

  prevEngineRPM = engineRPM;
  prevVehicleSpeed = vehicleSpeed;
  prevEngineCoolantTemp = engineCoolantTemp;

  if (engineRPMChanged)
  {
    // ENGINE RPM VALUE
    tft.fillRect(60, 200, 220, 60, BLACK);
    if (engineRPM > SHIFT_RPM_THRESHOLD)
    {
      tft.setTextColor(BLUE);
    }
    else
    {
      tft.setTextColor(WHITE);
    }
    tft.setTextSize(8);
    tft.setCursor(60, 200);
    tft.println(engineRPM_s);
  }

  if (vehicleSpeedChanged)
  {
    // VEHICLE SPEED VALUE
    tft.fillRect(60, 40, 220, 95, BLACK);
    tft.setTextSize(12);
    tft.setTextColor(WHITE);
    tft.setCursor(60, 40);
    tft.println(vehicleSpeed_s);
  }

  if (engineCoolantTempChanged)
  {
    // ENGINE COOLANT TEMP VALUE
    tft.fillRect(60, 330, 95, 60, BLACK);
    if (engineCoolantTemp < MIN_ENGINE_TEMP)
    {
      tft.setTextColor(RED);
    }
    else
    {
      tft.setTextColor(GREEN);
    }
    tft.setTextSize(8);
    tft.setCursor(60, 330);
    tft.println(engineCoolantTemp_s);
  }
}

// STARTUP SEQUENCE ANIMATION ON TFT
void startupSeq()
{
  tft.fillScreen(BLACK);
  delay(500);

  tft.setTextColor(WHITE);

  // M LOGO
  tft.setCursor(80, 80);
  tft.setTextSize(30);
  tft.println("M");

  // MAZDA / MIATA SUBTEXT
  delay(500);
  tft.setCursor(67, 300);
  tft.setTextSize(6);
  tft.print("MAZDA");
  tft.setCursor(67, 350);
  tft.print("MIATA");

  delay(1000);
  tft.fillScreen(BLACK);
}

// ATTEPMT TEXT
void attemptConn()
{
  tft.setCursor(10, 10);
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  tft.print("Trying to connect to OBD2 CAN bus... ");
  delay(1000);
}

// FAILED CONNECTION WITH OBD2 BUS TEXT
void failedConn()
{
  tft.setCursor(10, 50);
  tft.setTextColor(RED);
  tft.print("Failed! Retrying...");
  delay(1000);
  tft.fillRect(10, 50, tft.width() - 20, 80, BLACK);
}

// SUCCESFULL CONNECTION WITH OBD2 BUS TEXT
void succesfullConn()
{
  tft.setCursor(10, 50);
  tft.setTextColor(GREEN);
  tft.print("Connected!");
  tft.fillScreen(BLACK);
}

// VALUE AND VALUE SUBTEXT
void valueTitleText()
{
  // LINES
  // UNDER KM/H
  tft.drawLine(60, 190, 260, 190, WHITE);
  // UNDER RPM
  tft.drawLine(60, 320, 260, 320, WHITE);

  // SET DEFAULT CURSOR LOCATION AND TEXT COLOR
  tft.setTextSize(6);
  tft.setTextColor(WHITE);

  // VEHICLE SPEED SUBTEXT
  tft.setCursor(70, 140);
  tft.print("KM/H");

  // ENGINE RPM SUBTEXT
  tft.setCursor(70, 270);
  tft.print("RPM");

  // ENGINE COOLANT TEMP SUBTEXT
  tft.setCursor(160, 330);
  tft.setTextSize(4);
  tft.print((char)247);
  tft.setTextSize(6);
  tft.print("C");
}